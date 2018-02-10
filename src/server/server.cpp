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

#include "server.h"
#include <context/nwcontext.hpp>

Server::Server()
    : mWorlds(context.plugins, context.blocks), mNetwork(mWorlds)
{
    using namespace std::chrono;
    auto startTime = steady_clock::now();

    infostream << "Initializing plugins...";
    context.plugins.initializePlugins(nwPluginTypeServerOnly);

    // World
    mWorlds.addWorld("main_world");

    // Builtin Commands
    initBuiltinCommands();

    // Done
    auto doneTime = steady_clock::now();
    infostream << "Initialization done in " << duration_cast<milliseconds>(doneTime - startTime).count() << "ms!";
}

void Server::initBuiltinCommands()
{
    mCommands.registerCommand("help", { "internal","Help" }, [this](Command cmd)->CommandExecuteStat
    {
        std::string helpString = "\nAvailable commands:\n";
        for (const auto& command : mCommands.getCommandMap())
        {
            helpString += command.first + " - " + command.second.first.author + " : " + command.second.first.help + "\n";
        }
        return{ true, helpString };
    });
    mCommands.registerCommand("server.stop", { "internal","Stop the server." }, [this](Command cmd)->CommandExecuteStat
    {
        stop();
        return{ true, "" };
    });
    mCommands.registerCommand("conf.get", { "internal","Get one configuration item. Usage: conf.get <confname>" }, [this](Command cmd)->CommandExecuteStat
    {
        if (cmd.args.size() == 1)
        {
            auto keys = split(cmd.args[0], '.');
            Json now = getSettings();
            bool exist = true;
            for (auto key : keys)
            {
                auto iter = now.find(key);
                if (iter == now.end())
                {
                    exist = false;
                    break;
                }
                now = iter.value();
            }

            if (exist)
                return{ true, cmd.args[0] + " = " + now.dump() };
            else
                return{ false, "The configuration item does not exist." };
        }
        else
        {
            return{ false, "Usage: conf.get <confname>" };
        }
    });
    mCommands.registerCommand("conf.show", { "internal","Show the configuration." }, [this](Command cmd)->CommandExecuteStat
    {
        return{ true, getSettings().dump() };
    });
    mCommands.registerCommand("conf.save", { "internal","Save the configuration." }, [this](Command cmd)->CommandExecuteStat
    {
        writeJsonToFile(SettingsFilename, getSettings());
        return{ true,"Done!" };
    });
    mCommands.registerCommand("server.ups", { "internal","Show the ups." }, [this](Command cmd)->CommandExecuteStat
    {
        // TODO: Add UPS counter for server
        return{ true,"[Server UPS counter not finished yet!]" };
    });
    mCommands.registerCommand("chunks.count", { "internal","Show how many chunks are loaded" }, [this](Command cmd)->CommandExecuteStat
    {
        size_t sum = 0;
        for (auto&& world : mWorlds)
        {
            sum += world->getChunkCount();
        }
        return{ true,"Chunks loaded: " + std::to_string(sum) };
    });
}

void Server::run()
{
    // Network
    mNetwork.start(getJsonValue<std::string>(getSettings()["server"]["ip"], "127.0.0.1").c_str(),
                   getJsonValue<unsigned short>(getSettings()["server"]["port"], 31111));
    mNetwork.loop();
}

void Server::stop()
{
    mNetwork.close();
    mCommands.setRunningStatus(false);
}

Server::~Server()
{
    // TODO: Terminate here
}

// Normally it does nothing, just collects garbage :)
LocalTunnelServer::LocalTunnelServer()
{
}

// TEMP CODE!
// TODO : MultiPlayer
void LocalTunnelServer::run()
{
    mRuning.store(true);
    while (mRuning)
    {
        for (auto& w : mWorlds)
            w->updateChunkLoadStatus();
        std::this_thread::yield();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void LocalTunnelServer::stop()
{
    mRuning.store(false);
    mCommands.setRunningStatus(false);
}

World * LocalTunnelServer::getWorld(size_t id)
{
    return mWorlds.getWorld(id);
}

Chunk * LocalTunnelServer::getChunk(int x, int y, int z)
{
    World *world = mWorlds.getWorld(0); // TODO:Current world of player.
    Chunk* chunk;
    if (!world->isChunkLoaded({ x, y, z }))
        chunk = world->addChunk({ x, y, z });
    else
        chunk = &world->getChunk({ x, y, z });
    Assert(chunk);
    return chunk;
}

void LocalTunnelServer::login(const char *, const char *)
{
}
