/*
* NEWorld: A free game with similar rules to Minecraft.
* Copyright (C) 2016 NEWorld Team
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef NWAPIGUI_H_
#define NWAPIGUI_H_

#include "nwapicore.hpp"

extern "C"
{

    // API related to GUI or renderer

    typedef size_t NWtextureid;
    typedef void(*NWblockrenderfunc)(void* cthis, NWblockdata data, int x, int y, int z);

    struct NWblocktexture
    {
        NWtextureid right, left, top, bottom, front, back;
    };

    NWAPIENTRY NWtextureid NWAPICALL nwRegisterTexture(const char* filename);
    NWAPIENTRY void NWAPICALL nwSetBlockRenderFunc(size_t id, NWblockrenderfunc func);
    NWAPIENTRY void NWAPICALL nwUseDefaultBlockRenderFunc(size_t id, void* data);
}

#endif // !NWAPIGUI_H_

