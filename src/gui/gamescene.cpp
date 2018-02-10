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

#include <string.h>
#include <array>
#include <atomic>
#include <chrono>
#include "gamescene.h"
#include "window.h"
#include <engine/common.h>
#include "renderer/blockrenderer.h"
#include <context/nwcontext.hpp>

GameScene::GameScene(const std::string& name, std::shared_ptr<ClientGameConnection> connection,
           const Window& window):
    mWindow(window), mWorld(name, context.plugins, context.blocks), mPlayer(&mWorld), mConnection(connection)
{
    mWorld.setRenderDistance(2);
    mPlayer.setPosition(Vec3d(-16.0, 48.0, 32.0));
    mPlayer.setRotation(Vec3d(-45.0, -22.5, 0.0));

    // Initialize rendering
    mTexture = BlockTextureBuilder::buildAndFlush();
    BlockRendererManager::flushTextures();
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Initialize Widgets
    mGUIWidgets.addWidget(std::make_shared<WidgetCallback>("Debug", ImVec2(100, 200), [this]
    {
        mRateCounterScheduler.refresh();
        if (mRateCounterScheduler.shouldRun())
        {
            // Update FPS & UPS
            mFpsLatest = mFpsCounter;
            mUpsLatest = mUpsCounter;
            mFpsCounter = 0;
            mUpsCounter = 0;
            mRateCounterScheduler.increaseTimer();
        }
        ImGui::Text("NEWorld %s (v%u)", NEWorldVersionName, NEWorldVersion);
        ImGui::Text("FPS %d, UPS %d", mFpsLatest, mUpsLatest);
        ImGui::Text("Position: x %.1f y %.1f z %.1f", mPlayer.getPosition().x, mPlayer.getPosition().y, mPlayer.getPosition().z);
        ImGui::Text("GUI Widgets: %zu", mGUIWidgets.getSize());
        ImGui::Text("Chunks Loaded: %zu", mWorld.getChunkCount());
    }));

    // Initialize connection
    mConnection->setWorld(&mWorld);
    mConnection->connect();
    mConnection->waitForConnected();
    mConnection->login("test", "123456");
}

GameScene::~GameScene()
{
}

void GameScene::update()
{
    std::lock_guard<std::mutex> lock(mMutex);

    mUpsCounter++;

    constexpr double speed = 0.05;

    // TODO: Read keys from the configuration file
    auto state = Window::getKeyBoardState();
    if (state[SDL_SCANCODE_UP])
        mPlayer.accelerateRotation(Vec3d(1, 0.0, 0.0));
    if (state[SDL_SCANCODE_DOWN] && mPlayer.getRotation().x > -90)
        mPlayer.accelerateRotation(Vec3d(-1, 0.0, 0.0));
    if (state[SDL_SCANCODE_RIGHT])
        mPlayer.accelerateRotation(Vec3d(0.0, -1, 0.0));
    if (state[SDL_SCANCODE_LEFT])
        mPlayer.accelerateRotation(Vec3d(0.0, 1, 0.0));
    if (state[SDL_SCANCODE_W])
        mPlayer.accelerate(Vec3d(0.0, 0.0, -speed));
    if (state[SDL_SCANCODE_S])
        mPlayer.accelerate(Vec3d(0.0, 0.0, speed));
    if (state[SDL_SCANCODE_A])
        mPlayer.accelerate(Vec3d(-speed, 0.0, 0.0));
    if (state[SDL_SCANCODE_D])
        mPlayer.accelerate(Vec3d(speed, 0.0, 0.0));
    if (state[SDL_SCANCODE_SPACE])
        mPlayer.accelerate(Vec3d(0.0, 2 * speed, 0.0));
#ifdef NEWORLD_TARGET_MACOSX
    if (state[SDL_SCANCODE_LGUI] || state[SDL_SCANCODE_RGUI])
#else
    if (state[SDL_SCANCODE_LCTRL] || state[SDL_SCANCODE_RCTRL])
#endif
        mPlayer.accelerate(Vec3d(0.0, -2 * speed, 0.0));

    mPlayer.update();
    mWorld.sortChunkLoadUnloadList(Vec3i(mPlayer.getPosition()));
    mWorld.tryLoadChunks(*mConnection);
    mWorld.update();
    mWorld.renderUpdate(Vec3i(mPlayer.getPosition()));
    mGUIWidgets.update();
}

void GameScene::multiUpdate()
{
    const long long updateTimeout = 4000;
    mUpdateScheduler.refresh();
    if (mUpdateScheduler.getDeltaTimeMs() >= updateTimeout)
    {
        warningstream << "Can't keep up! " << mUpdateScheduler.getDeltaTimeMs() << "ms skipped.";
        mUpdateScheduler.sync();
    }
    while (mUpdateScheduler.shouldRun())
    {
        update();
        mUpdateScheduler.increaseTimer();
    }
}

void GameScene::render()
{
    std::lock_guard<std::mutex> lock(mMutex);

    mFpsCounter++;

    glClearColor(0.6f, 0.9f, 1.0f, 1.0f);
    glClearDepth(1.0f);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);

    mUpdateScheduler.refresh();
    double timeDelta = mUpdateScheduler.getDeltaTimeMs() / 1000.0 * UpdateFrequency;
    if (timeDelta > 1.0) timeDelta = 1.0;
    Vec3d playerRenderedPosition = mPlayer.getPosition() - mPlayer.getPositionDelta() * (1.0 - timeDelta);
    Vec3d playerRenderedRotation = mPlayer.getRotation() - mPlayer.getRotationDelta() * (1.0 - timeDelta);

    mTexture.bind(Texture::Texture2D);
    Renderer::clear();
    Renderer::setViewport(0, 0, mWindow.getWidth(), mWindow.getHeight());
    Renderer::restoreProj();
    Renderer::applyPerspective(70.0f, float(mWindow.getWidth()) / mWindow.getHeight(), 0.1f, 3000.0f);
    Renderer::restoreScale();
    Renderer::rotate(float(-playerRenderedRotation.x), Vec3f(1.0f, 0.0f, 0.0f));
    Renderer::rotate(float(-playerRenderedRotation.y), Vec3f(0.0f, 1.0f, 0.0f));
    Renderer::rotate(float(-playerRenderedRotation.z), Vec3f(0.0f, 0.0f, 1.0f));
    Renderer::translate(-playerRenderedPosition);

    // Render

    mWorld.render(Vec3i(mPlayer.getPosition()));

    // mPlayer.render();

    glDisable(GL_DEPTH_TEST);

    mGUIWidgets.render();
}
