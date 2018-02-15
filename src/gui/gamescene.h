// 
// GUI: gamescene.h
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

#ifndef GAME_H_
#define GAME_H_

#include <thread>
#include <mutex>
#include "client/player.h"
#include "widgetmanager.h"
#include <engine/common.h>
#include "window.h"
#include "renderer/texture.h"
#include "renderer/worldrenderer.h"

class GameScene {
public:
    // GameScene update frequency
    static constexpr int UpdateFrequency = 30;

    GameScene(const std::string& name, const Window& window);
    ~GameScene();

    void render();
private:
    void keyboardUpdateTask();

    const Window& mWindow;
    // Texture test
    Texture mTexture;
    // Player
    Player mPlayer;
    // Widget manager
    WidgetManager mGUIWidgets;
    // Update scheduler
    RateMeter mUpdateScheduler{UpdateFrequency};
    // Rate counters
    size_t mFpsCounter, mUpsCounter, mFpsLatest, mUpsLatest;
    // Current world
    World* mCurrentWorld;
    // World renderer
    WorldRenderer mWorldRenderer;

    RateMeter mRateCounterScheduler{1};
};

#endif // !GAME_H_
