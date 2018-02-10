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

#include "chunkrenderer.h"
#include "../../protocol/gen/s2c/2-Chunk_generated.h"
#include <world/worldclient.h>
VertexArray* target;
VertexArray ChunkRenderer::va0(262144, VertexFormat(2, 3, 0, 3));
VertexArray ChunkRenderer::va1(262144, VertexFormat(2, 3, 0, 3));
bool ChunkRenderer::mergeFace;

void ChunkRenderer::renderBlock(Chunk* chunk, BlockTexCoord coord[], const Vec3i& pos)
{
    Vec3i worldpos = chunk->getPosition() * Chunk::Size() + pos;

    BlockData curr = chunk->getBlock(pos);
    BlockData neighbors[6] =
    {
        pos.x == Chunk::Size() - 1 ? chunk->getWorld()->getBlock(Vec3i(worldpos.x + 1, worldpos.y, worldpos.z)) : chunk->getBlock(Vec3i(pos.x + 1, pos.y, pos.z)),
        pos.x == 0 ? chunk->getWorld()->getBlock(Vec3i(worldpos.x - 1, worldpos.y, worldpos.z)) : chunk->getBlock(Vec3i(pos.x - 1, pos.y, pos.z)),
        pos.y == Chunk::Size() - 1 ? chunk->getWorld()->getBlock(Vec3i(worldpos.x, worldpos.y + 1, worldpos.z)) : chunk->getBlock(Vec3i(pos.x, pos.y + 1, pos.z)),
        pos.y == 0 ? chunk->getWorld()->getBlock(Vec3i(worldpos.x, worldpos.y - 1, worldpos.z)) : chunk->getBlock(Vec3i(pos.x, pos.y - 1, pos.z)),
        pos.z == Chunk::Size() - 1 ? chunk->getWorld()->getBlock(Vec3i(worldpos.x, worldpos.y, worldpos.z + 1)) : chunk->getBlock(Vec3i(pos.x, pos.y, pos.z + 1)),
        pos.z == 0 ? chunk->getWorld()->getBlock(Vec3i(worldpos.x, worldpos.y, worldpos.z - 1)) : chunk->getBlock(Vec3i(pos.x, pos.y, pos.z - 1)),
    };

    // Right
    if (adjacentTest(curr, neighbors[0], chunk->getWorld()))
        target->addPrimitive(4,
    {
        coord[0].d[0], coord[0].d[1], 0.5f, 0.5f, 0.5f, pos.x + 1.0f, pos.y + 1.0f, pos.z + 1.0f,
        coord[0].d[0], coord[0].d[3], 0.5f, 0.5f, 0.5f, pos.x + 1.0f, pos.y + 0.0f, pos.z + 1.0f,
        coord[0].d[2], coord[0].d[3], 0.5f, 0.5f, 0.5f, pos.x + 1.0f, pos.y + 0.0f, pos.z + 0.0f,
        coord[0].d[2], coord[0].d[1], 0.5f, 0.5f, 0.5f, pos.x + 1.0f, pos.y + 1.0f, pos.z + 0.0f
    });

    // Left
    if (adjacentTest(curr, neighbors[1], chunk->getWorld()))
        target->addPrimitive(4,
    {
        coord[1].d[0], coord[1].d[1], 0.5f, 0.5f, 0.5f, pos.x + 0.0f, pos.y + 1.0f, pos.z + 0.0f,
        coord[1].d[0], coord[1].d[3], 0.5f, 0.5f, 0.5f, pos.x + 0.0f, pos.y + 0.0f, pos.z + 0.0f,
        coord[1].d[2], coord[1].d[3], 0.5f, 0.5f, 0.5f, pos.x + 0.0f, pos.y + 0.0f, pos.z + 1.0f,
        coord[1].d[2], coord[1].d[1], 0.5f, 0.5f, 0.5f, pos.x + 0.0f, pos.y + 1.0f, pos.z + 1.0f
    });

    // Top
    if (adjacentTest(curr, neighbors[2], chunk->getWorld()))
        target->addPrimitive(4,
    {
        /*0.0f, 0.0f, 1.0f, 1.0f, 1.0f, pos.x + 0.0f, pos.y + 1.0f, pos.z + 0.0f,
        0.0f, 1.0f, 1.0f, 1.0f, 1.0f, pos.x + 0.0f, pos.y + 1.0f, pos.z + 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 1.0f, pos.x + 1.0f, pos.y + 1.0f, pos.z + 1.0f,
        1.0f, 0.0f, 1.0f, 1.0f, 1.0f, pos.x + 1.0f, pos.y + 1.0f, pos.z + 0.0f,*/
        coord[2].d[0], coord[2].d[1], 1.0f, 1.0f, 1.0f, pos.x + 0.0f, pos.y + 1.0f, pos.z + 0.0f,
        coord[2].d[0], coord[2].d[3], 1.0f, 1.0f, 1.0f, pos.x + 0.0f, pos.y + 1.0f, pos.z + 1.0f,
        coord[2].d[2], coord[2].d[3], 1.0f, 1.0f, 1.0f, pos.x + 1.0f, pos.y + 1.0f, pos.z + 1.0f,
        coord[2].d[2], coord[2].d[1], 1.0f, 1.0f, 1.0f, pos.x + 1.0f, pos.y + 1.0f, pos.z + 0.0f
    });

    // Bottom
    if (adjacentTest(curr, neighbors[3], chunk->getWorld()))
        target->addPrimitive(4,
    {
        coord[3].d[0], coord[3].d[1], 1.0f, 1.0f, 1.0f, pos.x + 0.0f, pos.y + 0.0f, pos.z + 1.0f,
        coord[3].d[0], coord[3].d[3], 1.0f, 1.0f, 1.0f, pos.x + 0.0f, pos.y + 0.0f, pos.z + 0.0f,
        coord[3].d[2], coord[3].d[3], 1.0f, 1.0f, 1.0f, pos.x + 1.0f, pos.y + 0.0f, pos.z + 0.0f,
        coord[3].d[2], coord[3].d[1], 1.0f, 1.0f, 1.0f, pos.x + 1.0f, pos.y + 0.0f, pos.z + 1.0f
    });

    // Front
    if (adjacentTest(curr, neighbors[4], chunk->getWorld()))
        target->addPrimitive(4,
    {
        coord[4].d[0], coord[4].d[1], 0.7f, 0.7f, 0.7f, pos.x + 0.0f, pos.y + 1.0f, pos.z + 1.0f,
        coord[4].d[0], coord[4].d[3], 0.7f, 0.7f, 0.7f, pos.x + 0.0f, pos.y + 0.0f, pos.z + 1.0f,
        coord[4].d[2], coord[4].d[3], 0.7f, 0.7f, 0.7f, pos.x + 1.0f, pos.y + 0.0f, pos.z + 1.0f,
        coord[4].d[2], coord[4].d[1], 0.7f, 0.7f, 0.7f, pos.x + 1.0f, pos.y + 1.0f, pos.z + 1.0f
    });

    // Back
    if (adjacentTest(curr, neighbors[5], chunk->getWorld()))
        target->addPrimitive(4,
    {
        coord[5].d[0], coord[5].d[1], 0.7f, 0.7f, 0.7f, pos.x + 1.0f, pos.y + 1.0f, pos.z + 0.0f,
        coord[5].d[0], coord[5].d[3], 0.7f, 0.7f, 0.7f, pos.x + 1.0f, pos.y + 0.0f, pos.z + 0.0f,
        coord[5].d[2], coord[5].d[3], 0.7f, 0.7f, 0.7f, pos.x + 0.0f, pos.y + 0.0f, pos.z + 0.0f,
        coord[5].d[2], coord[5].d[1], 0.7f, 0.7f, 0.7f, pos.x + 0.0f, pos.y + 1.0f, pos.z + 0.0f
    });
}

ChunkRenderer::ChunkRenderer(Chunk *chunk) :mBuffer(), mBufferTrans()
{
    va0.clear();
    va1.clear();
    if (mergeFace)
    {
        // TODO: merge face rendering
    }
    else
    {
        Vec3i tmp;
        for (tmp.x = 0; tmp.x < Chunk::Size(); ++tmp.x)
            for (tmp.y = 0; tmp.y < Chunk::Size(); ++tmp.y)
                for (tmp.z = 0; tmp.z < Chunk::Size(); ++tmp.z)
                {
                    BlockData b = chunk->getBlock(tmp);
                    target = (chunk->getWorld()->getType(b.getID()).isTranslucent()) ? &va1 : &va0;
                    BlockRendererManager::render(b.getID(), chunk, tmp);
                }
    }

    mBuffer.update(va0);
    mBufferTrans.update(va1);
}


