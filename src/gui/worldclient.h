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

#ifndef WORLDCLIENT_H_
#define WORLDCLIENT_H_

#include <array>
#include <unordered_map>
#include "renderer/chunkrenderer.h"
#include "engine/nwstdlib/nworderedlist.hpp"

class ClientGameConnection;

const int MaxChunkRenderCount = 4;
constexpr int MaxChunkLoadCount = 64, MaxChunkUnloadCount = 64;

class WorldClient : public World
{
public:
    WorldClient(const std::string& name, const PluginManager& plugins, const BlockManager& blocks)
        : World(name, plugins, blocks)
    {
    }

    WorldClient(World&& world) : World(std::move(world))
    {
    }

    void setRenderDistance(int x)
    {
        mRenderDist = x;
        mLoadRange = x + 1;
    }

    // Build/Destroy VBO
    void renderUpdate(const Vec3i& position);
    // Render all chunks
    size_t render(const Vec3i& position) const;
    // Find the nearest chunks in load range to load, fartherest chunks out of load range to unload
    void sortChunkLoadUnloadList(const Vec3i& centerPos);

    void tryLoadChunks(ClientGameConnection& conn);

private:
    // Ranges
    int mRenderDist = 0, mLoadRange = 0;
    // Chunk Renderers
    std::unordered_map<Chunk*, ChunkRenderer> mChunkRenderers;
    // Chunk load list [position, distance]
    PODOrderedList<int, Vec3i, MaxChunkLoadCount> mChunkLoadList;
    // Render build list
    PODOrderedList<int, Chunk*, MaxChunkRenderCount> mChunkRenderList;
    // Chunk unload list [pointer, distance]
    PODOrderedList<int, Chunk*, MaxChunkUnloadCount, std::greater> mChunkUnloadList;
    bool neighbourChunkLoadCheck(const Vec3i& pos);
    static constexpr Vec3i middleOffset() noexcept
    { return Vec3i(Chunk::Size() / 2 - 1, Chunk::Size() / 2 - 1, Chunk::Size() / 2 - 1); }
};

#endif // !WORLDCLIENT_H_
