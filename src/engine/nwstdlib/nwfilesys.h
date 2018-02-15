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

#ifdef NEWORLD_TARGET_WINDOWS
    #include <io.h>
    #include <direct.h>
#else
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/stat.h>
    #include <sys/dir.h>
#endif

namespace FileSystem
{
    inline bool exists(const std::string& path)
    {
#ifdef NEWORLD_TARGET_WINDOWS
        return _access(path.c_str(), 0) == 0;
#else
        return access(path.c_str(), 0) == 0;
#endif
    }

    inline void createDirectory(const std::string& path)
    {
#ifdef NEWORLD_TARGET_WINDOWS
        _mkdir(path.c_str());
#else
        mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif
    }

    inline void forInDirectory(const std::string& path, std::function<void(std::string)> callback)
    {
#ifdef NEWORLD_TARGET_WINDOWS
        WIN32_FIND_DATA ffd;
        HANDLE hFind = FindFirstFileA((path + "\\*").c_str(), &ffd);
        do
        {
            if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;
            callback(path + "\\" + ffd.cFileName);
        }
        while (FindNextFileA(hFind, &ffd) != 0);
        FindClose(hFind);
#else
        auto pDir = opendir(path.c_str());
        struct dirent *ent;
        while ((ent = readdir(pDir)) != nullptr)
        {
            if (ent->d_type & DT_DIR) continue;
            callback(path + ent->d_name);
        }
#endif
    }
}
