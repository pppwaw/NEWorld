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

#include "taskdispatcher.hpp"
#include "chunkservice.hpp"
#include <chrono>

void TaskDispatcher::worker(size_t threadID) {
    debugstream << "Worker thread " << threadID << " initialized.";
    while (!mShouldExit) {
        // A tick starts
        RateMeter meter{ 60 };
        meter.sync();

        const size_t currentRoundNumber = mRoundNumber;
        // Process read-only work.
        for (auto i = threadID; i < mReadOnlyTasks.size(); i += mThreadNumber) {
            mReadOnlyTasks[i]->task(mChunkService);
        }

        // Finish the tick
        meter.refresh();
        mTimeUsed[threadID] = meter.getDeltaTimeMs();

        // The last finished thread is responsible to do writing jobs
        if (mNumberOfUnfinishedThreads.fetch_sub(1) == 1) { // All other threads have finished?
            for (const auto& task : mReadWriteTasks) {
                task->task(mChunkService);
            }

            // ...and finish up!
            mReadOnlyTasks.clear();
            mReadWriteTasks.clear();
            for (auto& task : mRegularReadOnlyTasks)
                mNextReadOnlyTasks.emplace_back(task->clone());
            for (auto& task : mRegularReadWriteTasks)
                mNextReadWriteTasks.emplace_back(task->clone());
            std::swap(mReadOnlyTasks, mNextReadOnlyTasks);
            std::swap(mReadWriteTasks, mNextReadWriteTasks);
            // TODO: UPS limits should apply here
            //using namespace std::chrono_literals;
            //std::this_thread::sleep_for(10ms);

            while (!meter.shouldRun()){
                std::this_thread::yield();
                meter.refresh();
            }

            // Time to move to next tick!
            // Notify other threads that we are good to go
            mNumberOfUnfinishedThreads = mThreadNumber;
            ++mRoundNumber;
        }
        else {
            // Wait for other threads...
            while (mRoundNumber == currentRoundNumber)
                std::this_thread::yield();
        }
    }
    debugstream << "Worker thread " << threadID << " exited.";
}
