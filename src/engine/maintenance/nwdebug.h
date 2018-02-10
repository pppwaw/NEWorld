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

#include "nwlogger.hpp"
#include <stdexcept>

#define NEWORLD_DEBUG
// Assertion uses C++ exception
inline void AssertFunc(bool expr, const char* file, const char* fname, int line)
{
    if (!expr)
    {
        fatalstream << "Assertion failed!";
        fatalstream << "At line " << line << " in \"" << file << "\", function " << fname;
        throw std::runtime_error("Assertion failed!");
    }
}

#ifdef NEWORLD_DEBUG
    #define Assert(expr) AssertFunc((expr) != 0, __FILE__, __FUNCTION__, __LINE__)
#else
    #define Assert(expr) nullptr
#endif

// A notice that would cause compilation error 2333
#undef assert
#define assert(expr) static_assert(false, "Do not #include <cassert> or #include <assert.h>! Use Assert(expression) instead."); (void)(expr);

