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
#include "client/player.h"
#include "../renderdetector.hpp"
#include <GL/glew.h>

size_t WorldRenderer::render(const Vec3i& position) const {
    // TODO: can be optimized by adding an array to store chunks need to be rendered
    //       so that we can avoid calculate chebyshevDistance twice.
    Vec3i chunkpos = World::getChunkPos(position);
    size_t renderedChunks = 0;
    for (auto&& c : mChunkRenderers) {
        if (chunkpos.chebyshevDistance(c.first) <= mRenderDist) {
            renderedChunks++;
            c.second.render(c.first);
        }
    }
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for (auto&& c : mChunkRenderers) {
        if (chunkpos.chebyshevDistance(c.first) <= mRenderDist) { c.second.renderTrans(c.first); }
    }
    glDisable(GL_BLEND);
    return renderedChunks;
}

void WorldRenderer::registerTask(ChunkService& chunkService, Player& player) noexcept {
    chunkService.getTaskDispatcher().addRegularReadOnlyTask(
        std::make_unique<RenderDetectorTask>(*this, mWorld.getWorldID(), player)
    );
}
