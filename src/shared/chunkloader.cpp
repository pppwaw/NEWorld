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

#include "chunkloader.h"
#include "pluginmanager.h"

void NWAPICALL DefaultChunkGen(const Vec3i*, BlockData* blocks, int32_t daylightBrightness)
{
    // This is the default terrain generator. Use this when no generators were loaded from plugins.
    for (int x = 0; x < ChunkSize; x++)
        for (int z = 0; z < ChunkSize; z++)
            for (int y = 0; y < ChunkSize; y++)
                blocks[x*ChunkSize*ChunkSize + y*ChunkSize + z] = BlockData(0, daylightBrightness, 0);
}

bool ChunkGeneratorLoaded = false;
ChunkGenerator *ChunkGen = &DefaultChunkGen;

void ChunkLoader::build(int daylightBrightness) const
{
    (*ChunkGen)(&m_chunk.getPosition(), m_chunk.getBlocks(), daylightBrightness);
}
