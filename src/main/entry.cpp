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

#include "worldgen.h"
#include "api/nwapigui.hpp"

int32_t GrassID, RockID, DirtID, SandID, WaterID;

int32_t registerBlock(const char* name, bool solid, bool translucent, bool opaque,
    int explodepower, int hardness)
{
    NWblocktype block;
    block.blockname = name;
    block.solid = solid;
    block.translucent = translucent;
    block.opaque = opaque;
    block.explodePower = explodepower;
    block.hardness = hardness;
    return nwRegisterBlock(&block);
}

extern "C"
{

    NWAPIEXPORT NWplugindata* NWAPICALL getInfo()
    {
        auto plugin = new NWplugindata();
        plugin->pluginName = "NEWorld Core";
        plugin->authorName = "INFINIDEAS";
        plugin->internalName = "infinideas.neworld.main.core";
        plugin->pluginType = nwPluginTypeCore;
        return plugin;
    }

    // Main function
    NWAPIEXPORT void NWAPICALL init(NWplugintype type)
    {
        nwRegisterChunkGenerator(generator);
        GrassID = registerBlock("Grass", true, false, true, 0, 2);
        RockID = registerBlock("Rock", true, false, true, 0, 2);
        DirtID = registerBlock("Dirt", true, false, true, 0, 2);
        SandID = registerBlock("Sand", true, false, true, 0, 2);
        WaterID = registerBlock("Water", false, true, false, 0, 2);
        if (type & nwPluginTypeGUI)
        {
            NWtextureid id[] =
            {
                nwRegisterTexture("./res/blocks/grass_top.png"),
                nwRegisterTexture("./res/blocks/grass_round.png"),
                nwRegisterTexture("./res/blocks/dirt.png"),
                nwRegisterTexture("./res/blocks/rock.png"),
                nwRegisterTexture("./res/blocks/sand.png"),
                nwRegisterTexture("./res/blocks/water.png")
            };
            NWblocktexture grass{ id[1], id[1], id[0], id[2], id[1], id[1] };
            NWblocktexture rock{ id[3], id[3], id[3], id[3], id[3], id[3] };
            NWblocktexture dirt{ id[2], id[2], id[2], id[2], id[2], id[2] };
            NWblocktexture sand{ id[4], id[4], id[4], id[4], id[4], id[4] };
            NWblocktexture water{ id[5], id[5], id[5], id[5], id[5], id[5] };
            nwUseDefaultBlockRenderFunc(GrassID, &grass);
            nwUseDefaultBlockRenderFunc(RockID, &rock);
            nwUseDefaultBlockRenderFunc(DirtID, &dirt);
            nwUseDefaultBlockRenderFunc(SandID, &sand);
            nwUseDefaultBlockRenderFunc(WaterID, &water);
        }
    }

    // Unload function
    NWAPIEXPORT void NWAPICALL unload()
    {
    }
}
