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

#ifndef WORLD_H_
#define WORLD_H_

#include <string>
#include <memory>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <world/nwchunk.h>
#include <engine/common.h>

class PluginManager;

class World : public NonCopyable
{
public:
    World(const std::string& name, const PluginManager& plugins, const BlockManager& blocks)
        : mName(name), mID(0), mPlugins(plugins), mBlocks(blocks), mDaylightBrightness(15), mChunks(1024)
    {
    }

    World(World&& rhs) noexcept
        : mName(std::move(rhs.mName)), mID(rhs.mID), mPlugins(rhs.mPlugins), mBlocks(rhs.mBlocks),
        mDaylightBrightness(rhs.mDaylightBrightness), mChunks(std::move(rhs.mChunks))
    {
    }
    virtual ~World() = default;

    ////////////////////////////////////////
    // World Properties
    ////////////////////////////////////////
    const std::string& getWorldName() const noexcept { return mName; }
    size_t getWorldID() const noexcept { return mID; }
    int getDaylightBrightness() const noexcept { return mDaylightBrightness; }

    ////////////////////////////////////////
    // Chunk Management
    ////////////////////////////////////////
    using ChunkIterator = ChunkManager::iterator;
    using ChunkReference = ChunkManager::reference;
    // Raw Access
    ChunkManager& getChunks() noexcept { return mChunks; }
    const ChunkManager& getChunks() const noexcept { return mChunks; }
    // Alias declearations for Chunk management
    size_t getChunkCount() const { return mChunks.size(); }
    ChunkReference getChunk(const Vec3i& ChunkPos) { return mChunks[ChunkPos]; }
    bool isChunkLoaded(const Vec3i& ChunkPos) const noexcept { return mChunks.isLoaded(ChunkPos); }
    void deleteChunk(const Vec3i& ChunkPos) { mChunks.erase(ChunkPos); }
    static int getChunkAxisPos(int pos) { return ChunkManager::getAxisPos(pos); }
    static Vec3i getChunkPos(const Vec3i& pos) { return ChunkManager::getPos(pos); }
    static int getBlockAxisPos(int pos) { return ChunkManager::getBlockAxisPos(pos); }
    static Vec3i getBlockPos(const Vec3i& pos) { return ChunkManager::getBlockPos(pos); }
    BlockData getBlock(const Vec3i& pos) const { return mChunks.getBlock(pos); }
    void setBlock(const Vec3i& pos, BlockData block) { mChunks.setBlock(pos, block); }
    auto insertChunk(const Vec3i& pos, ChunkManager::data_t&& ptr) { return mChunks.insert(pos, std::move(ptr)); }
    auto resetChunk(const Vec3i& pos, Chunk* ptr) { return mChunks.reset(pos, ptr); }
    template <typename... ArgType, typename Func>
    void doIfChunkLoaded(const Vec3i& ChunkPos, Func func, ArgType&&... args)
    {
        mChunks.doIfLoaded(ChunkPos, func, std::forward<ArgType>(args)...);
    };

    // Add Chunk
    virtual Chunk* addChunk(const Vec3i& chunkPos, ChunkOnReleaseBehavior::Behavior behv = ChunkOnReleaseBehavior::Behavior::Release) 
    {
        return insertChunk(chunkPos, std::move(ChunkManager::data_t(new Chunk(chunkPos, *this), ChunkOnReleaseBehavior(behv))))->second.get();
    }


    ////////////////////////////////////////
    // BlockType Management
    ////////////////////////////////////////
    const BlockManager& getBlockTypes() const { return mBlocks; }
    const BlockType& getType(int id) const { return mBlocks[id]; }

    NWCOREAPI std::vector<AABB> getHitboxes(const AABB& range) const;

    // Main update
    NWCOREAPI void update();

    NWCOREAPI void updateChunkLoadStatus();

protected:
    // World name
    std::string mName;
    std::mutex mMutex;
    // World ID
    size_t mID;
    static size_t IDCount;
    // Loaded plugins
    const PluginManager& mPlugins;
    // Loaded blocks
    const BlockManager& mBlocks;
    // All Chunks (Chunk array)
    ChunkManager mChunks;
    int mDaylightBrightness;
};

class WorldManager
{
public:
    WorldManager(PluginManager& plugins, BlockManager& blocks) :
        mPlugins(plugins), mBlocks(blocks)
    {
    }

    ~WorldManager()
    {
        mWorlds.clear();
    }

    void clear()
    {
        mWorlds.clear();
    }

    World* addWorld(const std::string& name)
    {
        mWorlds.emplace_back(new World(name, mPlugins, mBlocks));
        return mWorlds[mWorlds.size() - 1].get();
    }

    std::vector<std::unique_ptr<World>>::iterator begin()
    {
        return mWorlds.begin();
    }

    std::vector<std::unique_ptr<World>>::iterator end()
    {
        return mWorlds.end();
    }

    World* getWorld(const std::string& name)
    {
        for (auto&& world : *this)
        {
            if (world->getWorldName() == name) return world.get();
        }
        return nullptr;
    }

    World* getWorld(size_t id)
    {
        for (auto&& world : *this)
        {
            if (world->getWorldID() == id) return world.get();
        }
        return nullptr;
    }

private:
    std::vector<std::unique_ptr<World>> mWorlds;
    PluginManager& mPlugins;
    BlockManager& mBlocks;
};

#endif // !WORLD_H_
