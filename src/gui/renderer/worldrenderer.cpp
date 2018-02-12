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

#include "worldrenderer.h"

size_t WorldRenderer::render(const Vec3i& position) const
{
    // TODO: can be optimized by adding an array to store chunks need to be rendered
    //       so that we can avoid calculate chebyshevDistance twice.
    Vec3i chunkpos = World::getChunkPos(position);
    size_t renderedChunks = 0;
    for (auto&& c : mChunkRenderers)
    {
        if (chunkpos.chebyshevDistance(c.first->getPosition()) <= mRenderDist) {
            renderedChunks++;
            c.second.render(c.first->getPosition());
        }
    }
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for (auto&& c : mChunkRenderers)
    {
        if (chunkpos.chebyshevDistance(c.first->getPosition()) <= mRenderDist){
            c.second.renderTrans(c.first->getPosition());
        }
    }
    glDisable(GL_BLEND);
    return renderedChunks;
}


bool WorldRenderer::neighbourChunkLoadCheck(const Vec3i& pos)
{
    constexpr std::array<Vec3i, 6> delta
    {
            Vec3i(1, 0, 0), Vec3i(-1, 0, 0), Vec3i(0, 1, 0),
            Vec3i(0,-1, 0), Vec3i(0, 0, 1), Vec3i(0, 0,-1)
    };
    for (auto&& p : delta)
        if (!mWorld.isChunkLoaded(pos + p))
            return false;
    return true;
}
