#include "taskdispatcher.hpp"
#include "chunkservice.hpp"

void TaskDispatcher::worker(size_t threadID) {
    while (!mShouldExit) {
        // A tick starts

        // Process read-only work.
        for (auto i = threadID; i < mReadOnlyTasks.size(); i += mThreadNumber) {
            mReadOnlyTasks[threadID].task(mChunkService.getWorlds());
        }

        // Finish the tick
        --mNumberOfUnfinishedThreads;

        // The last finished thread is responsible to do writing jobs
        if (mNumberOfUnfinishedThreads == 0) { // All other threads have finished?
            for (const auto& task : mReadWriteTasks) {
                task.task(mChunkService.getWorlds());
            }

            // ...and finish up!
            mReadOnlyTasks.clear();
            mReadWriteTasks.clear();
            for (auto& task : mRegularReadOnlyTasks) mReadOnlyTasks.emplace_back(task);
            for (auto& task : mReadWriteTasks) mReadWriteTasks.emplace_back(task);
            std::swap(mReadOnlyTasks, mNextReadOnlyTasks);
            std::swap(mReadWriteTasks, mNextReadWriteTasks);
            // TODO: UPS limits should apply here

            // Time to move to next tick!
            // Notify other threads that we are good to go
            mNumberOfUnfinishedThreads = mThreadNumber;
        }
        else {
            // Wait for other threads...
            while (mNumberOfUnfinishedThreads != mThreadNumber)
                std::this_thread::yield();
        }
    }
}
