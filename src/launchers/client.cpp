// 
// NEWorld: client.cpp
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

#include <iostream>
#include <string>
#include <climits>
#include <engine/common.h>

typedef void NWAPICALL MainFunction(int, char**);

#if defined(NEWORLD_TARGET_WINDOWS)
constexpr const char* GUIDllName = "GUI.dll";
#elif defined(NEWORLD_TARGET_MACOSX)
    constexpr const char* GUIDllName = "libGUI.dylib";
#elif defined(NEWORLD_TARGET_LINUX)
    constexpr const char* GUIDllName = "libGUI.so";
#endif

int main(int argc, char** argv) {
    getSettings();
    Logger::addFileSink("./log/", "launcher");
    std::string file = argc == 1 ? GUIDllName : argv[1];
    debugstream << "Load:" << file;
    Library(file).get<MainFunction>("cmain")(argc, argv);
}
