// 
// nwcore: pluginmanager.cpp
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

#include "pluginmanager.h"
#include "Common/StringUtils.h"
#include "Common/Filesystem.h"
#include "Common/Logger.h"

PluginManager::PluginManager() {
    infostream << "Start to load plugins...";
    size_t counter = 0;
    const filesystem::path path = "./plugins/";
    if (filesystem::exists(path)) {
        for (auto&& file : filesystem::directory_iterator(path)) {
            auto suffix = file.path().extension().string();
            strToLower(suffix);
            if (suffix == LibSuffix) {
                debugstream << "Loading:" << file.path().string();
                if (loadPlugin(file.path().string()))
                    counter++;
            }
        };
    }
    infostream << counter << " plugin(s) loaded";
}

PluginManager::~PluginManager() { mPlugins.clear(); }

void PluginManager::initializePlugins(NWplugintype flag) {
    for (auto&& plugin : mPlugins)
        plugin.init(flag);
}

bool PluginManager::loadPlugin(const std::string& filename) {
    mPlugins.emplace_back(filename);
    Plugin& plugin = mPlugins[mPlugins.size() - 1];

    if (!plugin.isLoaded()) {
        mPlugins.pop_back();
        warningstream << "Failed to load plugin from \"" << filename << "\", skipping";
        return false;
    }
    else {
        infostream << "Loaded plugin \"" << plugin.getData().pluginName << "\"["
            << plugin.getData().internalName
            << "], authored by \"" << plugin.getData().authorName << "\"";
        return true;
    }
}
