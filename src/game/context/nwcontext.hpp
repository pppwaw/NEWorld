#pragma once
#include "./../plugin/pluginmanager.h"
#include "sync_service/world/nwblock.h"
#include "engine/nwrpc/nwrpc.hpp"

struct GameContext
{
    BlockManager blocks;
    PluginManager plugins;
    RPC rpc;
};

NWCOREAPI extern GameContext context;
