/*
* NEWorld: A free game with similar rules to Minecraft.
* Copyright (C) 2016 NEWorld Team
*
* This file is part of NEWorld.
* NEWorld is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* NEWorld is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with NEWorld.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once
#include <thread>
#include <vector>
#include "sync_service/world/world.h"
#include <any>

class ChunkService;

// TODO: we can add a `finished` flag in DEBUG mode
//       to verify that all tasks are indeed processed.
/**
 * \brief This type of tasks will be executed concurrently.
 *        Note that "ReadOnly" here is with respect to chunks
 *        data specifically. However please be aware of
 *        thread safety when you write something other than
 *        chunks.
 */
struct NWCOREAPI ReadOnlyTask {
    std::function<void(const ChunkService&)> task;
};
/**
 * \brief This type of tasks will be executed in one thread.
 *        Thus, it is safe to do write opeartions inside
 *        without the need to worry thread safety.
 */
struct NWCOREAPI ReadWriteTask {
    std::function<void(ChunkService&)> task;
};
/**
 * \brief This type of tasks will be executed in main thread.
 *        Thus, it is safe to call OpenGL function inside.
 */
struct NWCOREAPI RenderTask {
    std::function<void(const ChunkService&)> task;
    std::any data;
};

template <class TaskType>
struct NWCOREAPI RegularTask {
    std::function<TaskType()> taskGenerator;
};
using RegularReadOnlyTask = RegularTask<ReadOnlyTask>;
using RegularReadWriteTask = RegularTask<ReadWriteTask>;

class NWCOREAPI TaskDispatcher {
public:
    /**
     * \brief Initialize the dispatcher and start threads.
     * \param threadNumber The number of threads in the thread pool
     * \param chunkService the chunk service that the dispatcher binds to
     */
    TaskDispatcher(size_t threadNumber, ChunkService& chunkService)
        : mThreadNumber(threadNumber), mChunkService(chunkService) {
    }
    
    ~TaskDispatcher() {
        mShouldExit = true;
        for (auto& thread : mThreads) thread.join();
    }

    void start() {
        mNumberOfUnfinishedThreads = mThreadNumber;
        for (size_t i = 0; i < mThreadNumber; ++i)
            mThreads.emplace_back([this, i]() {worker(i); });
    }

    // TODO: NEED FIX! NOT THREAD SAFE!
    void addReadOnlyTask(const ReadOnlyTask& task) noexcept {
        mNextReadOnlyTasks.emplace_back(task);
    }
    void addReadWriteTask(const ReadWriteTask& task) noexcept {
        mNextReadWriteTasks.emplace_back(task);
    }
    void addRenderTask(const RenderTask& task) noexcept {
        mNextRenderTasks.emplace_back(task);
    }
    void addRegularReadOnlyTask(const RegularReadOnlyTask& task) noexcept {
        mRegularReadOnlyTasks.emplace_back(task);
    }
    void addRegularReadWriteTask(const RegularReadWriteTask& task) noexcept {
        mRegularReadWriteTasks.emplace_back(task);
    }

    /**
     * \brief Process render tasks.
     *        This function should be called from the main thread.
     */
    void processRenderTasks() {
        for (auto& task : mRenderTasks) task.task(mChunkService);
        mRenderTasks.clear();
        std::swap(mRenderTasks, mNextRenderTasks);
    }

private:
    void worker(size_t threadID);

    std::vector<ReadOnlyTask> mReadOnlyTasks, mNextReadOnlyTasks;
    std::vector<ReadWriteTask> mReadWriteTasks, mNextReadWriteTasks;
    std::vector<RenderTask> mRenderTasks, mNextRenderTasks;
    std::vector<RegularReadOnlyTask> mRegularReadOnlyTasks;
    std::vector<RegularReadWriteTask> mRegularReadWriteTasks;
    std::vector<std::thread> mThreads;
    size_t mThreadNumber;
    std::atomic<size_t> mNumberOfUnfinishedThreads;
    std::atomic<bool> mShouldExit{ false };

    ChunkService& mChunkService;
};
