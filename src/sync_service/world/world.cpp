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

#include <world/world.h>

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

void World::update()
{
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
