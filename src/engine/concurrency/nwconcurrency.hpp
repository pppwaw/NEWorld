// 
// nwcore: nwconcurrency.hpp
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

#pragma once

class Task {
public:
    enum class Priority { VeryHigh, High, Normal, Low, VeryLow };

    Task(std::chrono::steady_clock::duration duration, Priority priority)
        : mDuration(duration), mPriority(priority) {}

    auto remainingTime(std::chrono::steady_clock::time_point tp) { return mLastCall + mDuration - tp; }

    void call(std::chrono::steady_clock::time_point tp) {
        mExecuting.store(true);
        std::this_thread::sleep_for(remainingTime(tp));
        this->execute();
        mExecuting.store(false);
    }

    bool isExectuing() { return mExecuting; }
    virtual ~Task() = default;
    virtual void execute() = 0;
private:
    std::chrono::steady_clock::duration mDuration;
    std::chrono::steady_clock::time_point mLastCall;
    Priority mPriority;
    std::atomic_bool mExecuting;
};

class FunctionTask : public Task {
public:
    FunctionTask(std::chrono::steady_clock::duration duration, Priority priority,
                 const std::function<void()>& callable) :
        mCallable(callable), Task(duration, priority) {}

    virtual void execute() override { mCallable(); }
private:
    std::function<void()> mCallable;
};

class TaskManager {
public:
    using TaskPointer = std::unique_ptr<Task>;

    void callNext() {
        auto time = std::chrono::steady_clock::now();
        auto next = mTasks.front().get();
        for (auto&& x : mTasks)
            if (!(x->isExectuing()))
                if (next->remainingTime(time) > x->remainingTime(time))
                    next = x.get();
        next->call(time);
    }

    void addTask(TaskPointer&& tsk) { mTasks.push_back(std::move(tsk)); }

    void start() {
        mRunning.store(true);
        while (mRunning)
            callNext();
    }

    void stop() { mRunning.store(false); }
private:
    std::atomic_bool mRunning;
    std::vector<TaskPointer> mTasks;
};
