// 
// Core: taskdispatcher.cpp
// NEWorld: A Free Game with Similar Rules to Minecraft.
// Copyright (C) 2015-2018 NEWorld Team
// 
// NEWorld is free software: you can redistribute it and/or modify it 
// under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation, either version 3 of the License, or 
// (at your option) any later version.
// 
// NEWorld is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General 
// Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with NEWorld.  If not, see <http://www.gnu.org/licenses/>.
//

#include <vector>
#include "taskdispatcher.hpp"
#include "Common/RateController.h"
#include <Core/Threading/SpinLock.h>
#include <Core/Threading/Micro/Timer.h>
#include <Core/Threading/Micro/ThreadPool.h>
#include <Core/Utilities/TempAlloc.h>

namespace {
    enum class DispatchMode : int {
        Read, ReadWrite, None, Render
    };

    ChunkService* gService{nullptr};
    std::atomic_bool gEnter{false};
    thread_local DispatchMode gThreadMode{DispatchMode::None};

    std::vector<std::unique_ptr<ReadOnlyTask>> gReadOnlyTasks, gNextReadOnlyTasks, gRegularReadOnlyTasks;
    std::vector<std::unique_ptr<ReadWriteTask>> gReadWriteTasks, gNextReadWriteTasks, gRegularReadWriteTasks;
    std::vector<std::unique_ptr<RenderTask>> gRenderTasks, gNextRenderTasks;
    std::vector<int64_t> gTimeUsed;
    int64_t gTimeUsedRWTasks;

    SpinLock gReadLock{}, gWriteLock{}, gRenderLock{};

    void ExecuteWriteTasks() noexcept {
        gThreadMode = DispatchMode::ReadWrite;
        for (const auto& task : gReadWriteTasks) { task->task(*gService); }
        gThreadMode = DispatchMode::None;
    }

    void PrepareNextReadOnly() {
        gReadOnlyTasks.clear();
        gReadLock.Enter();
        for (auto& task : gRegularReadOnlyTasks)
            gNextReadOnlyTasks.emplace_back(task->clone());
        std::swap(gReadOnlyTasks, gNextReadOnlyTasks);
        gReadLock.Leave();
    }

    void PrepareNextReadWrite() {
        gReadWriteTasks.clear();
        gWriteLock.Enter();
        for (auto& task : gRegularReadWriteTasks)
            gNextReadWriteTasks.emplace_back(task->clone());
        std::swap(gReadWriteTasks, gNextReadWriteTasks);
        gWriteLock.Leave();
    }

    template <class T>
    [[nodiscard]] auto CountElapsedMs(const T& start) {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()-start).count();
    }

    void ReadOnlyTaskFinal() noexcept {
        const auto start = std::chrono::steady_clock::now();
        ExecuteWriteTasks();
        PrepareNextReadOnly();
        PrepareNextReadWrite();
        gEnter.store(false);
        gTimeUsedRWTasks = CountElapsedMs(start);
    }

    class ReadPoolTask : public AInstancedExecTask {
    public:
        void Exec(uint32_t instance) noexcept override {
            InitTick();
            const auto completed = DrainReads();
            FinishTick(instance);
            CompleteTasks(completed);
        }

        static void Reset() noexcept {
            mCounter = mDone = 0;
            mTCount = gReadOnlyTasks.size();
        }

        static void CompleteTasks(int count) noexcept {
            if (ReadPoolTask::mDone.fetch_add(count)+count==ReadPoolTask::mTCount) {
                ReadOnlyTaskFinal();
            }
        }

        static void AddTasks(int count) noexcept { mTCount.fetch_add(count); }

        static int CountCurrentRead() noexcept { return mTCount.load(); }
    private:
        void InitTick() noexcept {
            meter.sync();
            gThreadMode = DispatchMode::Read;
        }

        void FinishTick(uint32_t instance) noexcept {
            gThreadMode = DispatchMode::None;
            gTimeUsed[instance] = meter.getDeltaTimeMs();
        }

        [[nodiscard]] static int DrainReads() noexcept {
            int localCount = 0;
            for (;;) {
                const auto i = mCounter.fetch_add(1);
                if (i<gReadOnlyTasks.size()) {
                    gReadOnlyTasks[i]->task(*gService);
                    ++localCount;
                }
                else return localCount;
            }
        }
        RateController meter{30};

        inline static std::atomic_int mCounter{}, mDone{}, mTCount{};
    } gReadPoolTask;

    class MainTimer : public CycleTask {
    public:
        MainTimer() noexcept
                :CycleTask(std::chrono::milliseconds(33)) { }

        void OnTimer() noexcept override {
            auto val = gEnter.exchange(true);
            if (!val) {
                ReadPoolTask::Reset();
                ThreadPool::Spawn(&gReadPoolTask);
            }
        }
    } gMainTimer;

    class ReadSingleTask : public IExecTask {
    public:
        explicit ReadSingleTask(std::unique_ptr<ReadOnlyTask> task) noexcept
                :mTask(std::move(task)) { }
        void Exec() noexcept override {
            gThreadMode = DispatchMode::Read;
            mTask->task(*gService);
            gThreadMode = DispatchMode::None;
            mTask.reset();
            ReadPoolTask::CompleteTasks(1);
            Temp::Delete(this);
        }

        [[nodiscard]] static IExecTask* Create(std::unique_ptr<ReadOnlyTask> task) noexcept {
            ReadPoolTask::AddTasks(1);
            return Temp::New<ReadSingleTask>(std::move(task));
        }
    private:
        std::unique_ptr<ReadOnlyTask> mTask;
    };
}

void TaskDispatch::boot(ChunkService& service) {
    gTimeUsed.resize(ThreadPool::CountThreads());
    gService = std::addressof(service);
    gEnter.store(false);
    gMainTimer.Enable();
}

void TaskDispatch::shutdown() noexcept {
    gMainTimer.Disable();
}

void TaskDispatch::addNow(std::unique_ptr<ReadOnlyTask> task) noexcept {
    if (gThreadMode==DispatchMode::Read) {
        ThreadPool::Enqueue(ReadSingleTask::Create(std::move(task)));
    }
    else {
        TaskDispatch::addNext(std::move(task));
    }
}

void TaskDispatch::addNext(std::unique_ptr<ReadOnlyTask> task) noexcept {
    gReadLock.Enter();
    gNextReadOnlyTasks.emplace_back(std::move(task));
    gReadLock.Leave();
}

void TaskDispatch::addNow(std::unique_ptr<ReadWriteTask> task) noexcept {
    if (gThreadMode==DispatchMode::ReadWrite) {
        task->task(*gService);
    }
    else {
        TaskDispatch::addNext(std::move(task));
    }
}

void TaskDispatch::addNext(std::unique_ptr<ReadWriteTask> task) noexcept {
    gWriteLock.Enter();
    gNextReadWriteTasks.emplace_back(std::move(task));
    gWriteLock.Leave();
}

void TaskDispatch::addNow(std::unique_ptr<RenderTask> task) noexcept {
    if (gThreadMode==DispatchMode::Render) {
        task->task(*gService);
    }
    else {
        TaskDispatch::addNext(std::move(task));
    }
}

void TaskDispatch::addNext(std::unique_ptr<RenderTask> task) noexcept {
    gRenderLock.Enter();
    gNextRenderTasks.emplace_back(std::move(task));
    gRenderLock.Leave();
}

void TaskDispatch::handleRenderTasks() noexcept {
    for (auto& task : gRenderTasks) task->task(*gService);
    gRenderTasks.clear();
    gRenderLock.Enter();
    std::swap(gRenderTasks, gNextRenderTasks);
    gRenderLock.Leave();
}

void TaskDispatch::addRegular(std::unique_ptr<ReadOnlyTask> task) noexcept {
    gReadLock.Enter();
    gRegularReadOnlyTasks.emplace_back(std::move(task));
    gReadLock.Leave();
}

void TaskDispatch::addRegular(std::unique_ptr<ReadWriteTask> task) noexcept {
    gWriteLock.Enter();
    gRegularReadWriteTasks.emplace_back(std::move(task));
    gWriteLock.Leave();
}

int TaskDispatch::countWorkers() noexcept {
    return ThreadPool::CountThreads();
}

int64_t TaskDispatch::getReadTimeUsed(int i) noexcept {
    return gTimeUsed.size()>i ? gTimeUsed[i] : 0;
}

int64_t TaskDispatch::getRWTimeUsed() noexcept {
    return gTimeUsedRWTasks;
}

int TaskDispatch::getRegularReadTaskCount() noexcept {
    return gRegularReadOnlyTasks.size();
}

int TaskDispatch::getRegularReadWriteTaskCount() noexcept {
    return gRegularReadWriteTasks.size();
}

int TaskDispatch::getReadTaskCount() noexcept {
    return ReadPoolTask::CountCurrentRead();
}

int TaskDispatch::getReadWriteTaskCount() noexcept {
    return gReadWriteTasks.size();
}
