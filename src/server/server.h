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

#ifndef SERVER_H_
#define SERVER_H_

#include <memory>
#include <thread>
#include <world/world.h>
#include <engine/common.h>
#include <plugin/pluginmanager.h>
#include <network/networkmanager.h>

constexpr int UpdateInterval = 1000 / 60, GlobalUpdateInterval = 1000 / 60; // unit: ms

class Server
{
// Basic Network Server
public:
    Server();
    virtual void run();
    virtual void stop();
    ~Server();
protected:
    void initBuiltinCommands();

    RateMeter mRateCounterScheduler{1};

    // Component managers
    WorldManager mWorlds;
    NetworkManager mNetwork;
    CommandManager mCommands;
};

class LocalTunnelServer : public Server
{
public:
    LocalTunnelServer();
    void run() override;
    void stop() override;
    World* getWorld(size_t id);
    Chunk* getChunk(int x, int y, int z);
    void login(const char*, const char*);
private:
    std::atomic_bool mRuning{false};
};
#endif
