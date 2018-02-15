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

#include <engine/common.h>
#include "pluginmanager.h"

PluginManager::PluginManager()
{
    infostream << "Start to load plugins...";
    using namespace FileSystem;
    size_t counter = 0;
    std::string path = "./plugins/";
    if (exists(path))
    {
        forInDirectory(path, [&, this](std::string filename)
        {
            std::string suffix = filename.substr(filename.size() - std::string(LibSuffix).size());
            strtolower(suffix);
            if (suffix != LibSuffix) return; //TODO: FIXME: may ignore linux plugins
            debugstream << "Loading:" << filename;
            if(loadPlugin(filename))
                counter++;
        });
    }
    infostream << counter << " plugin(s) loaded";
}

PluginManager::~PluginManager()
{
    mPlugins.clear();
}

void PluginManager::initializePlugins(NWplugintype flag)
{
    for (auto&& plugin : mPlugins)
        plugin.init(flag);
}

bool PluginManager::loadPlugin(const std::string& filename)
{
    mPlugins.push_back(std::move(Plugin(filename)));
    Plugin& plugin = mPlugins[mPlugins.size() - 1];

    if (!plugin.isLoaded())
    {
        mPlugins.pop_back();
        warningstream << "Failed to load plugin from \"" << filename << "\", skipping";
        return false;
    }
    else
    {
        infostream << "Loaded plugin \"" << plugin.getData().pluginName << "\"["
            << plugin.getData().internalName
            << "], authored by \"" << plugin.getData().authorName << "\"";
        return true;
    }
}
