// 
// nwcore: nwapicore.hpp
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

#ifndef NWAPICORE_H_
#define NWAPICORE_H_

#include "nwapi_internal.hpp"

extern "C" {

    // NEWorld constants

    const int NWChunkSize = 32;
    const int32_t NWAirID = 0;

    // NEWorld structures

    struct NWvec3i {
        int32_t x, y, z;
    };

    enum NWplugintype {
        nwPluginTypeNone = 0,
        nwPluginTypeGUI = 1,
        nwPluginTypeCLI = 2,
        nwPluginTypeCore = 3
    };

    struct NWplugindata {
        const char* pluginName;
        const char* authorName;
        const char* internalName;
        int32_t pluginType;
    };

    struct NWblockdata {
        uint32_t id : 12;
        uint32_t brightness : 4;
        uint32_t state : 16;
    };

    struct NWblocktype {
        const char* blockname;
        bool solid;
        bool translucent;
        bool opaque;
        int32_t explodePower;
        int32_t hardness;
    };

    typedef void* NWchunk;
    typedef void* NWworld;

    // NEWorld APIs
    NWAPIENTRY NWchunk NWAPICALL nwCreateChunk(NWworld, const NWvec3i*);
    NWAPIENTRY NWblockdata* NWAPICALL nwGetBlocksInChunk(NWchunk);
    NWAPIENTRY void NWAPICALL nwDeleteChunk(NWchunk);

    NWAPIENTRY size_t NWAPICALL nwRegisterBlock(const NWblocktype*);
    NWAPIENTRY void NWAPICALL nwLog(char* str);

    typedef void NWAPICALL NWchunkgenerator(const NWvec3i*, NWblockdata*, int32_t);
    NWAPIENTRY size_t NWAPICALL nwRegisterChunkGenerator(NWchunkgenerator* const generator);

}

#endif // !NWAPICORE_H_
