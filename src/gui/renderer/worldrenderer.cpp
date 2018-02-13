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

size_t WorldRenderer::render(const Vec3i& position) const
{
    // TODO: can be optimized by adding an array to store chunks need to be rendered
    //       so that we can avoid calculate chebyshevDistance twice.
    Vec3i chunkpos = World::getChunkPos(position);
    size_t renderedChunks = 0;
    for (auto&& c : mChunkRenderers)
    {
        if (chunkpos.chebyshevDistance(c.first) <= mRenderDist) {
            renderedChunks++;
            c.second.render(c.first);
        }
    }
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for (auto&& c : mChunkRenderers)
    {
        if (chunkpos.chebyshevDistance(c.first) <= mRenderDist){
            c.second.renderTrans(c.first);
        }
    }
    glDisable(GL_BLEND);
    return renderedChunks;
}

void WorldRenderer::registerTask(ChunkService & chunkService, Player & player) noexcept {
    ReadOnlyTask renderDetectorTask{
        [&](const ChunkService& cs) {
            renderDetector(cs, mWorld.getWorldID(), player.getPosition());
        }
    };

    chunkService.getTaskDispatcher().addRegularReadOnlyTask(
        { [=]() {return renderDetectorTask; } }
    );
}

void WorldRenderer::VAGenerate(const Chunk * chunk) {
    // TODO: only a workaround.
    Vec3i chunkPosition = chunk->getPosition();
    RenderTask task;
    task.data = (void*)(new ChunkRenderData());
    task.task = {
        [=](const ChunkService&)
    {
        ChunkRenderData* crd = static_cast<ChunkRenderData*>(task.data);
        VBOGenerateTask(chunkPosition, *crd);
        delete crd;
    }
    };
    static_cast<ChunkRenderData*>(task.data)->generate(chunk);
    chunkService.getTaskDispatcher().addRenderTask(task);
}

void WorldRenderer::VBOGenerateTask(const Vec3i & position, ChunkRenderData& crd) {
    mWorld.getChunks()[position].setUpdated(false);
    mChunkRenderers.insert(std::make_pair(position, ChunkRenderer(crd)));
}

void WorldRenderer::renderDetector(const ChunkService & cs, size_t currentWorldID, Vec3d playerPosition) {
    // TODO: improve performance by adding multiple instances of this and set a step when itering the chunks.
    // Render build list
    //PODOrderedList<int, Chunk*, MaxChunkRenderCount> chunkRenderList;
    Vec3i chunkpos = World::getChunkPos(playerPosition);
    for (const auto& chunk : chunkService.getWorlds().getWorld(currentWorldID)->getChunks()) {
        auto chunkPosition = chunk.second->getPosition();
        // In render range, pending to render
        if (chunk.second->isUpdated() &&
            chunkpos.chebyshevDistance(chunkPosition) <= mRenderDist)
        {
            if (neighbourChunkLoadCheck(chunkPosition)) {
                VAGenerate(chunk.second.get());
            }
        }
        else
        {
            // TODO: Unload unneeded VBO.
            //       We can't do it here since it's not thread safe
            /*
            auto iter = mChunkRenderers.find(chunkPosition);
            if (iter != mChunkRenderers.end())
            mChunkRenderers.erase(iter);
            */
        }
    }
}


bool WorldRenderer::neighbourChunkLoadCheck(const Vec3i& pos) const
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
