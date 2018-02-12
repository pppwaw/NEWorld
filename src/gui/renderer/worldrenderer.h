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
#include "chunkrenderer.h"
#include "engine/nwstdlib/nworderedlist.hpp"
#include "sync_service/chunkservice.hpp"

const int MaxChunkRenderCount = 4;

/**
 * \brief Manage the VBO of a world. It includes ChunkRenderer.
 */
class WorldRenderer
{
public:
    WorldRenderer(const World& world, int renderDistance)
        : mWorld(world), mRenderDist(renderDistance)
    {
    }

    // Render all chunks
    size_t render(const Vec3i& position) const;


    void renderDetector(const ChunkService& chunkService, size_t currentWorldID, Vec3d playerPosition) {

        // Render build list
        PODOrderedList<int, Chunk*, MaxChunkRenderCount> chunkRenderList;
        Vec3i chunkpos = World::getChunkPos(playerPosition);
        for (const auto& chunk : chunkService.getWorlds().getWorld(currentWorldID)->getChunks()) {

            // In render range, pending to render
            if (chunkpos.chebyshevDistance(chunk.second->getPosition()) <= mRenderDist)
            {
                if (mChunkRenderers.find(chunk.second.get()) == mChunkRenderers.end() &&
                    neighbourChunkLoadCheck(chunk.second->getPosition()))
                    chunkRenderList.insert((chunk.second->getPosition() * Chunk::Size() + middleOffset() - position).lengthSqr(), chunk.second.get());
            }
            else
            {
                auto iter = mChunkRenderers.find(chunk.second.get());
                if (iter != mChunkRenderers.end())
                    mChunkRenderers.erase(iter);
            }
        }

        for (auto&& op : mChunkRenderList)
        {
            op.second->setUpdated(false);
            mChunkRenderers.insert(std::pair<Chunk*, ChunkRenderer>(op.second, std::move(ChunkRenderer(op.second))));
        }
        mChunkRenderList.clear();
    }

private:
    const World& mWorld;
    // Ranges
    int mRenderDist = 0;
    // Chunk Renderers
    std::unordered_map<Chunk*, ChunkRenderer> mChunkRenderers;

    bool neighbourChunkLoadCheck(const Vec3i& pos);
};



#endif // !WORLDCLIENT_H_
