// 
// nwcore: nwratemeter.h
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

#include <chrono>

class RateMeter {
    static auto getTimeNow() noexcept { return std::chrono::steady_clock::now(); }
public:
    explicit RateMeter(const int limit = 0) noexcept : mLimit(limit), mOnlineTimer(getTimeNow()),
                                                       mOfflineTimer(getTimeNow()) { }

    void refresh() noexcept {
        mOnlineTimer = getTimeNow();
        mElapsed = mOnlineTimer - mOfflineTimer;
    }

    void sync() noexcept {
        mOfflineTimer = mOnlineTimer;
        mElapsed = std::chrono::steady_clock::duration(0);
    }

    auto getDeltaTimeMs() const noexcept {
        return std::chrono::duration_cast<std::chrono::milliseconds>(mElapsed).count();
    }

    // Notice: this function will not call refresh()!
    bool shouldRun() const noexcept {
        if (mLimit) {
            const auto stdDelta = std::chrono::milliseconds(1000 / mLimit);
            const auto deltaTime = mOnlineTimer - mOfflineTimer;
            return stdDelta <= deltaTime;
        }
        return true;
    }

    void increaseTimer() noexcept {
        if (mLimit) {
            mOfflineTimer += std::chrono::milliseconds(1000 / mLimit);
            mElapsed = mOnlineTimer - mOfflineTimer;
        }
    }

private:
    int mLimit;
    std::chrono::steady_clock::duration mElapsed{};
    std::chrono::steady_clock::time_point mOnlineTimer, mOfflineTimer;
};
