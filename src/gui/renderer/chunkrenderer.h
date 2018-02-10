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

#ifndef CHUNKCLIENT_H_
#define CHUNKCLIENT_H_

#include <atomic>
#include <memory>
#include <world/nwchunk.h>
#include <world/world.h>
#include <renderer/renderer.h>
#include "blockrenderer.h"

class WorldClient;

namespace s2c
{
    struct Chunk;
}

class ChunkRenderer : public NonCopyable
{
public:
    ChunkRenderer() = default;
    ChunkRenderer(Chunk* chunk);
    ChunkRenderer(ChunkRenderer&& rhs):
            mBuffer(std::move(rhs.mBuffer)), mBufferTrans(std::move(rhs.mBufferTrans)) {}
    ChunkRenderer& operator=(ChunkRenderer&& rhs)
    {
        mBuffer = std::move(rhs.mBuffer);
        mBufferTrans = std::move(rhs.mBufferTrans);
        return *this;
    }

    // Draw call
    void render(const Vec3i& c) const
    {
        if (!mBuffer.isEmpty())
        {
            Renderer::translate(Vec3f(c * Chunk::Size()));
            mBuffer.render();
            Renderer::translate(Vec3f(-c * Chunk::Size()));
        }
    }

    void renderTrans(const Vec3i& c) const
    {
        if (!mBufferTrans.isEmpty())
        {
            Renderer::translate(Vec3f(c * Chunk::Size()));
            mBufferTrans.render();
            Renderer::translate(Vec3f(-c * Chunk::Size()));
        }
    }

    // Render default block
    static void renderBlock(Chunk* chunk, BlockTexCoord coord[], const Vec3i& pos);
private:
    // Vertex buffer object
    VertexBuffer mBuffer, mBufferTrans;
    static bool mergeFace;
    static VertexArray va0, va1;
    static bool adjacentTest(BlockData a, BlockData b, World* world) noexcept
    {
        return a.getID() != 0 && !world->getType(b.getID()).isOpaque() && !(a.getID() == b.getID());
    }
};
#endif // !CHUNKCLIENT_H_
