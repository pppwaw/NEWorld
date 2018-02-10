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

#include <world/world.h>
#include <algorithm>

void NWAPICALL DefaultChunkGen(const Vec3i*, BlockData* blocks, int32_t daylightBrightness)
{
    // This is the default terrain generator. Use this when no generators were loaded from plugins.
    for (int x = 0; x < Chunk::Size() * Chunk::Size() * Chunk::Size(); ++x)
        blocks[x] = BlockData(0, daylightBrightness, 0);
}

bool Chunk::ChunkGeneratorLoaded = false;
ChunkGenerator *Chunk::ChunkGen = &DefaultChunkGen;

Chunk::Chunk(const Vec3i& position, class World& world) : mPosition(position), mWorld(&world)
{
    std::unique_lock<std::mutex> lock(mMutex);
    build(mWorld->getDaylightBrightness());
    constexpr std::array<Vec3i, 6> delta
    {
        Vec3i(1, 0, 0), Vec3i(-1, 0, 0),
        Vec3i(0, 1, 0), Vec3i(0,-1, 0),
        Vec3i(0, 0, 1), Vec3i(0, 0,-1)
    };
    for (auto&& p : delta)
        mWorld->doIfChunkLoaded(getPosition() + p, [](Chunk& chk)
    {
        chk.setUpdated(true);
    });
}

void Chunk::build(int daylightBrightness)
{
    (*ChunkGen)(&getPosition(), getBlocks(), daylightBrightness);
    setUpdated(true);
}
