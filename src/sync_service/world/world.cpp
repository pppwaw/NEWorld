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

#include "world.h"
#include "gui/gamescene.h"
#include "engine/nwstdlib/nworderedlist.hpp"

constexpr int MaxChunkLoadCount = 64, MaxChunkUnloadCount = 64;

size_t World::IDCount = 0;

std::vector<AABB> World::getHitboxes(const AABB& range) const
{
    std::vector<AABB> res;
    Vec3i curr;
    for (curr.x = int(floor(range.min.x)); curr.x < int(ceil(range.max.x)); curr.x++)
        for (curr.y = int(floor(range.min.y)); curr.y < int(ceil(range.max.y)); curr.y++)
            for (curr.z = int(floor(range.min.z)); curr.z < int(ceil(range.max.z)); curr.z++)
            {
                // TODO: BlockType::getAABB
                if (!isChunkLoaded(getChunkPos(curr)))
                    continue;
                if (getBlock(curr).getID() == 0)
                    continue;
                Vec3d currd = curr;
                res.push_back(AABB(currd, currd + Vec3d(1.0, 1.0, 1.0)));
            }
    return res;
}

void World::updateChunkLoadStatus()
{
    std::unique_lock<std::mutex> lock(mMutex);
    for (auto iter = mChunks.begin(); iter != mChunks.end();)
    {
        if (iter->second->checkReleaseable())
            iter = mChunks.erase(iter);
        else
            ++iter;
    }
}

static constexpr Vec3i middleOffset() noexcept
{
    return Vec3i(Chunk::Size() / 2 - 1, Chunk::Size() / 2 - 1, Chunk::Size() / 2 - 1);
}

/**
* \brief Find the nearest chunks in load range to load,
*        fartherest chunks out of load range to unload.
* \param world the world to load or unload chunks
* \param centerPos the center position
* \param loadRange chunk load range
* \param loadList (Output) Chunk load list [position, distance]
* \param unloadList (Output) Chunk unload list [pointer, distance]
*/
static void generateLoadUnloadList(
    const World& world, const Vec3i& centerPos, int loadRange,
    PODOrderedList<int, Vec3i, MaxChunkLoadCount>& loadList,
    PODOrderedList<int, Chunk*, MaxChunkUnloadCount, std::greater>& unloadList)
{
    // centerPos to chunk coords
    Vec3i centerCPos = World::getChunkPos(centerPos);

    for (auto&& chunk : world.getChunks())
    {
        Vec3i curPos = chunk.second->getPosition();
        // Out of load range, pending to unload
        if (centerCPos.chebyshevDistance(curPos) > loadRange)
            unloadList.insert((curPos * Chunk::Size() + middleOffset() - centerPos).lengthSqr(), chunk.second.get());
    }

    for (int x = centerCPos.x - loadRange; x <= centerCPos.x + loadRange; x++)
        for (int y = centerCPos.y - loadRange; y <= centerCPos.y + loadRange; y++)
            for (int z = centerCPos.z - loadRange; z <= centerCPos.z + loadRange; z++)
                // In load range, pending to load
                if (!world.isChunkLoaded(Vec3i(x, y, z)))
                    loadList.insert((Vec3i(x, y, z) * Chunk::Size() + middleOffset() - centerPos).lengthSqr(), Vec3i(x, y, z));
}

class AddToWorldTask : public ReadWriteTask {
public:
    /**
     * \brief Add a constructed chunk into world.
     * \param cs The chunk service
     * \param worldID the target world's id
     * \param chunk the target chunk
     */
    AddToWorldTask(size_t worldID, std::unique_ptr<Chunk, ChunkOnReleaseBehavior> chunk)
        : mWorldId(worldID), mChunk(std::move(chunk)) {
    }

    void task(ChunkService& cs) override {
        auto world = chunkService.getWorlds().getWorld(mWorldId);
        auto chunkPos = mChunk->getPosition();
        world->insertChunk(chunkPos, std::move(mChunk));
        constexpr std::array<Vec3i, 6> delta
        {
            Vec3i(1, 0, 0), Vec3i(-1, 0, 0),
            Vec3i(0, 1, 0), Vec3i(0,-1, 0),
            Vec3i(0, 0, 1), Vec3i(0, 0,-1)
        };
        for (auto&& p : delta)
            world->doIfChunkLoaded(chunkPos + p, [](Chunk& chk)
        {
            chk.setUpdated(true);
        });
    }

private:
    size_t mWorldId;
    std::unique_ptr<Chunk, ChunkOnReleaseBehavior> mChunk;
};

class BuildOrLoadChunkTask : public ReadOnlyTask {
public:
    /**
     * \brief Given a chunk, it will try to load it or build it
     * \param world the target world
     * \param chunkPosition the position of the chunk
     */
    BuildOrLoadChunkTask(const World& world, Vec3i chunkPosition)
        : mWorld(world), mChunkPosition(chunkPosition) {
        
    }

    void task(const ChunkService& cs) override {
        // TODO: avoid using raw pointer directly somehow... if possible
        ChunkManager::data_t chunk;
        if (false) { // TODO: should try to load from local first

        }
        else { // Not found: build it!
            chunk = ChunkManager::data_t(new Chunk(mChunkPosition, mWorld),
                ChunkOnReleaseBehavior::Behavior::Release);
        }
        // Add addToWorldTask
        chunkService.getTaskDispatcher().addReadWriteTask(
            std::make_unique<AddToWorldTask>(mWorld.getWorldID(), std::move(chunk))
        );
    }
private:
    const World& mWorld;
    Vec3i mChunkPosition;
};

class LoadUnloadDetectorTask : public ReadOnlyTask {
public:
    LoadUnloadDetectorTask(World& world, const Player& player): mPlayer(player), mWorld(world) {
    }

    void task(const ChunkService& cs) override {
        PODOrderedList<int, Vec3i, MaxChunkLoadCount> loadList;
        PODOrderedList<int, Chunk*, MaxChunkUnloadCount, std::greater> unloadList;

        // TODO: make the load range adjustable
        generateLoadUnloadList(mWorld, mPlayer.getPosition(), 4, loadList, unloadList);

        for (auto& loadPos : loadList) {
            if (chunkService.isAuthority()) {
                // add BuildOrLoadChunkTask
                chunkService.getTaskDispatcher().addReadOnlyTask(
                    std::make_unique<BuildOrLoadChunkTask>(mWorld, loadPos.second)
                );
            }
            else {
                // TODO: Get chunk via RPC
            }
        }
        for (auto& unloadChunk : unloadList) {
            // TODO: add a unload task.

        }
    }

    std::unique_ptr<ReadOnlyTask> clone() override {
        return std::make_unique<LoadUnloadDetectorTask>(*this);
    }

private:
    const Player& mPlayer;
    const World& mWorld;
};

void World::registerChunkTasks(ChunkService& chunkService, Player& player) {
    // LoadUnloadDetectorTask
    chunkService.getTaskDispatcher().addRegularReadOnlyTask(
        std::make_unique<LoadUnloadDetectorTask>(*this, player)
    );
}
