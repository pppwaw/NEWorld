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

#ifndef GAME_H_
#define GAME_H_

#include <thread>
#include <mutex>
#include "player.h"
#include "widgetmanager.h"
#include <world/world.h>
#include <world/nwblock.h>
#include <world/worldclient.h>
#include <engine/common.h>
#include <renderer/texture.h>
#include <renderer/renderer.h>
#include <plugin/pluginmanager.h>
#include <network/network.h>
#include <network/clientgameconnection.h>
#include "window.h"

class GameScene
{
public:
    // GameScene update frequency
    static constexpr int UpdateFrequency = 30;

    GameScene(const std::string& name, std::shared_ptr<ClientGameConnection> connection,
         const Window& window);
    ~GameScene();

    void update();
    void multiUpdate();
    void render();
private:
    const Window& mWindow;
    // Texture test
    Texture mTexture;
    // World
    WorldClient mWorld;
    // Player
    Player mPlayer;
    // Widget manager
    WidgetManager mGUIWidgets;
    // Update scheduler
    RateMeter mUpdateScheduler{UpdateFrequency};
    // Rate counters
    int mFpsCounter, mUpsCounter, mFpsLatest, mUpsLatest;
    RateMeter mRateCounterScheduler{1};
    // GameScene connection
    std::shared_ptr<ClientGameConnection> mConnection;

    std::mutex mMutex;
};

#endif // !GAME_H_
