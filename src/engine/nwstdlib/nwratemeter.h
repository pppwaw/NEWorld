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

class RateMeter
{
public:
    explicit RateMeter(int limit = 0) : mOnlineTimer(getTimeNow()), mOfflineTimer(getTimeNow()), mLimit(limit)
    {
#ifdef NEWORLD_USE_WINAPI
        LARGE_INTEGER num;
        QueryPerformanceFrequency(&num);
        mFrequency = num.QuadPart;
#endif
    }

    void refresh()
    {
        mOnlineTimer = getTimeNow();
#ifdef NEWORLD_USE_WINAPI
        mDeltaTime = mOnlineTimer - mOfflineTimer;
#else
        mDeltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(mOnlineTimer - mOfflineTimer).count();
#endif
    }

    void sync()
    {
        mOfflineTimer = mOnlineTimer;
        mDeltaTime = 0;
    }

    double getDeltaTimeMs() const
    {
#ifndef NEWORLD_USE_WINAPI
        return mDeltaTime;
#else
        return 1000.0 * mDeltaTime / mFrequency;
#endif
    }

    // Notice: this function will not call refresh()!
    bool shouldRun() const
    {
        if (mLimit == 0) return true;
#ifndef NEWORLD_USE_WINAPI
        auto stdDelta = std::chrono::milliseconds(1000 / mLimit);
        auto deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(mOnlineTimer - mOfflineTimer);
#else
        double stdDelta = 1.0 / mLimit;
        double deltaTime = static_cast<double>(mOnlineTimer - mOfflineTimer) / mFrequency;
#endif
        return stdDelta <= deltaTime;
    }

    void increaseTimer()
    {
        if (mLimit == 0) return;
#ifndef NEWORLD_USE_WINAPI
        mOfflineTimer += std::chrono::milliseconds(1000 / mLimit);
        mDeltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(mOnlineTimer - mOfflineTimer).count();
#else
        mOfflineTimer += mFrequency / mLimit;
        mDeltaTime = mOnlineTimer - mOfflineTimer;
#endif
    }

private:
#ifndef NEWORLD_USE_WINAPI
    std::chrono::steady_clock::time_point mOnlineTimer;
    std::chrono::steady_clock::time_point mOfflineTimer;
    long long mDeltaTime = 0ll;

    static std::chrono::steady_clock::time_point getTimeNow()
    {
        return std::chrono::steady_clock::now();
    }
#else
    __int64 mOnlineTimer;
    __int64 mOfflineTimer;
    __int64 mFrequency;
    __int64 mDeltaTime = 0ll;

    static __int64 getTimeNow()
    {
        LARGE_INTEGER num;
        QueryPerformanceCounter(&num);
        return num.QuadPart;
    }
#endif

    int mLimit;
    bool mValid = false;
};
