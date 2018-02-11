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

class ChunkService;

// TODO: we can add a `finished` flag in DEBUG mode
//       to verify that all tasks are indeed processed.
struct ReadOnlyTask {
    std::function<void(const WorldManager&)> task;
};
struct ReadWriteTask {
    std::function<void(WorldManager&)> task;
};

class TaskDispatcher {
public:
    /**
     * \brief Initialize the dispatcher and start threads.
     * \param threadNumber The number of threads in the thread pool
     * \param chunkService the chunk service that the dispatcher binds to
     */
    TaskDispatcher(size_t threadNumber, ChunkService& chunkService)
        : mThreadNumber(threadNumber), mChunkService(chunkService) {
        mNumberOfUnfinishedThreads = threadNumber;
        for (size_t i = 0; i < threadNumber; ++i)
            mThreads.emplace_back([this, i]() {worker(i); });
    }
    
    ~TaskDispatcher() {
        mShouldExit = true;
        for (auto& thread : mThreads) thread.join();
    }

    void addReadOnlyTask(const ReadOnlyTask& task) noexcept {
        mNextReadOnlyTasks.emplace_back(task);
    }
    void addReadWriteTask(const ReadWriteTask& task) noexcept {
        mNextReadWriteTasks.emplace_back(task);
    }

private:
    void worker(size_t threadID);

    std::vector<ReadOnlyTask> mReadOnlyTasks, mNextReadOnlyTasks;
    std::vector<ReadWriteTask> mReadWriteTasks, mNextReadWriteTasks;
    std::vector<std::thread> mThreads;
    size_t mThreadNumber;
    std::atomic<size_t> mNumberOfUnfinishedThreads;
    std::atomic<bool> mShouldExit{ false };

    ChunkService& mChunkService;
};
