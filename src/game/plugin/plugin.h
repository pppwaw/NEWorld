// 
// nwcore: plugin.h
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

#ifndef PLUGIN_H_
#define PLUGIN_H_
#include <string>
#include <utility>
#include <engine/common.h>
#include <api/nwapicore.hpp>

// Single plugin
class Plugin : public NonCopyable {
public:
    explicit Plugin(const std::string& filename) : mStatus(-1) { loadFrom(filename); }

    Plugin(Plugin&& rhs) noexcept : mLib(std::move(rhs.mLib)), mData(rhs.mData), mStatus(rhs.mStatus) {
        rhs.mData = nullptr;
        rhs.mStatus = -1;
    }

    ~Plugin() { unload(); }

    int init(NWplugintype type);

    // Get plugin data
    const NWplugindata& getData() const { return *mData; }

    // Get load status
    int getStatus() const { return mStatus; }

    // Is loaded
    bool isLoaded() const { return mStatus == 0; }

    // Load plugin, return 0 for success
    int loadFrom(const std::string& filename);
    // Unload plugin
    void unload();

    bool isCompatible(NWplugintype type) const { return ((mData->pluginType & type) > 0); }

private:
    // Plugin DLL
    Library mLib;
    // Plugin Data
    const NWplugindata* mData;
    // Load status
    int mStatus = -1;
    NWplugintype mLoadStat = nwPluginTypeNone;
};

#endif // !PLUGIN_H_
