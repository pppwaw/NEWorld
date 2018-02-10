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

#include "neworld.h"
#include <engine/common.h>
#include <renderer/texture.h>
#include <context/nwcontext.hpp>
#include "window.h"
#include "gamescene.h"
NEWorld::NEWorld()
{
    // Initialize
    getSettings();
    infostream << "Initializing...";
    Window& window = Window::getInstance("NEWorld", 852, 480);
    Renderer::init();
    Texture::init();
    context.plugins.initializePlugins(nwPluginTypeShared);

    auto conn = std::make_shared<LocalConnection>();
        //getJsonValue<std::string>(getSettings()["server"]["ip"], "127.0.0.1"),
        //getJsonValue<unsigned short>(getSettings()["server"]["port"], 31111)),
    
    // Run
    constexpr const static int fps = 60;// TODO: read from settings
    constexpr const static double delayPerFrame = (1000/fps)-0.5;
    GameScene game("TestWorld", conn, window);
    while(!window.shouldQuit())
    {
        // Update
        window.pollEvents();
        game.multiUpdate();
        // Render
        window.newIMGUIFrame();
        game.render();
        Renderer::checkError();
        window.swapBuffers();
        SDL_Delay(delayPerFrame);
    }

    // Terminate
    infostream << "Terminating...";
    Texture::free();
}
