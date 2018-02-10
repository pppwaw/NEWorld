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

#include "worldclient.h"
#include <network/clientgameconnection.h>

void WorldClient::renderUpdate(const Vec3i& position)
{
    Vec3i chunkpos = getChunkPos(position);
    for (auto&& chunk : mChunks)
    {
        // In render range, pending to render
        if (chunkpos.chebyshevDistance(chunk.second->getPosition()) <= mRenderDist)
        {
            if ((mChunkRenderers.find(chunk.second.get()) == mChunkRenderers.end()) &&
                    neighbourChunkLoadCheck(chunk.second->getPosition()))
                mChunkRenderList.insert((chunk.second->getPosition() * Chunk::Size() + middleOffset() - position).lengthSqr(), chunk.second.get());
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

size_t WorldClient::render(const Vec3i& position) const
{
    Vec3i chunkpos = getChunkPos(position);
    size_t renderedChunks = 0;
    for (auto&& c : mChunkRenderers)
    {
        if (chunkpos.chebyshevDistance(c.first->getPosition()) <= mRenderDist)
            c.second.render(c.first->getPosition());
    }
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for (auto&& c : mChunkRenderers)
    {
        if (chunkpos.chebyshevDistance(c.first->getPosition()) <= mRenderDist)
            c.second.renderTrans(c.first->getPosition());
    }
    glDisable(GL_BLEND);
    return renderedChunks;
}

void WorldClient::sortChunkLoadUnloadList(const Vec3i& centerPos)
{
    // centerPos to chunk coords
    Vec3i centerCPos = getChunkPos(centerPos);

    for (auto&& chunk : mChunks)
    {
        Vec3i curPos = chunk.second->getPosition();
        // Out of load range, pending to unload
        if (centerCPos.chebyshevDistance(curPos) > mLoadRange)
            mChunkUnloadList.insert((curPos * Chunk::Size() + middleOffset() - centerPos).lengthSqr(), chunk.second.get());
    }

    for (int x = centerCPos.x - mLoadRange; x <= centerCPos.x + mLoadRange; x++)
        for (int y = centerCPos.y - mLoadRange; y <= centerCPos.y + mLoadRange; y++)
            for (int z = centerCPos.z - mLoadRange; z <= centerCPos.z + mLoadRange; z++)
                // In load range, pending to load
                if (!isChunkLoaded(Vec3i(x, y, z)))
                    mChunkLoadList.insert((Vec3i(x, y, z) * Chunk::Size() + middleOffset() - centerPos).lengthSqr(), Vec3i(x, y, z));
}

void WorldClient::tryLoadChunks(ClientGameConnection& conn)
{
    for (auto&& op : mChunkLoadList)
        conn.getChunk(op.second);
    for (auto&& op : mChunkUnloadList)
        deleteChunk(op.second->getPosition());
    mChunkLoadList.clear();
    mChunkUnloadList.clear();
}

bool WorldClient::neighbourChunkLoadCheck(const Vec3i& pos)
{
    constexpr std::array<Vec3i, 6> delta
    {
            Vec3i(1, 0, 0), Vec3i(-1, 0, 0), Vec3i(0, 1, 0),
            Vec3i(0,-1, 0), Vec3i(0, 0, 1), Vec3i(0, 0,-1)
    };
    for (auto&& p : delta)
        if (!isChunkLoaded(pos + p))
            return false;
    return true;
}
