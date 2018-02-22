// 
// nwcore: server.cpp
// NEWorld: A Free Game with Similar Rules to Minecraft.
// Copyright (C) 2015-2018 NEWorld Team
// 
// NEWorld is free software: you can redistribute it and/or modify it 
// under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation, either version 3 of the License, or 
// (at your option) any later version.
// 
// NEWorld is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General 
// Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with NEWorld.  If not, see <http://www.gnu.org/licenses/>.
// 

#include "server.h"
#include "game/context/nwcontext.hpp"
#include "sync_service/chunkservice.hpp"

Server::Server() {
    using namespace std::chrono;
    auto startTime = steady_clock::now();

    infostream << "Initializing nwcore plugins...";
    context.plugins.initializePlugins(nwPluginTypeCore);

    // Register chunk update event
    chunkService.getWorlds().addWorld("test world");

    // Done
    auto doneTime = steady_clock::now();
    infostream << "Initialization done in " << duration_cast<milliseconds>(doneTime - startTime).count() << "ms!";
}

// This function won't block the thread
void Server::run(uint16_t port, size_t threadNumber) {
    // Network
    context.rpc.enableServer(port);
    context.rpc.getServer().async_run(threadNumber);
    infostream << "Server RPC started. Thread number: " << threadNumber;
}

void Server::stop() { context.rpc.getServer().stop(); }

Server::~Server() {
    // TODO: Terminate here
}
