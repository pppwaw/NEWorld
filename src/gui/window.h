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

#ifndef WINDOW_H_
#define WINDOW_H_

#include <string>
#include <SDL2/SDL.h>
#include "imguihelper.h"

class Window
{
public:
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    void makeCurrentDraw() const
    {
        SDL_GL_MakeCurrent(mWindow, mContext);
    }

    void swapBuffers() const
    {
        SDL_GL_SwapWindow(mWindow);
    }

    static const Uint8* getKeyBoardState()
    {
        return SDL_GetKeyboardState(nullptr);
    }

    int getWidth() const
    {
        return mWidth;
    }

    int getHeight() const
    {
        return mHeight;
    }

    void pollEvents()
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            imguiHelper::processEvent(&e);
            switch (e.type)
            {
            case SDL_QUIT:
                mShouldQuit = true;
                break;
            case SDL_WINDOWEVENT:
                switch (e.window.event)
                {
                case SDL_WINDOWEVENT_RESIZED:
                case SDL_WINDOWEVENT_SIZE_CHANGED:
                    mWidth = e.window.data1;
                    mHeight = e.window.data2;
                    break;
                }
                break;
            }
        }
    }

    static Window& getInstance(const std::string& title="", int width=0, int height=0)
    {
        static Window win(title, width, height);
        return win;
    }

    bool shouldQuit() const
    {
        return mShouldQuit;
    }

    void newIMGUIFrame() const
    {
        imguiHelper::newFrame(mWindow);
    }

private:
    SDL_Window* mWindow = nullptr;
    std::string mTitle;
    int mWidth, mHeight;
    bool mShouldQuit = false;

    Window(const std::string& title, int width, int height);
    ~Window();

    SDL_GLContext mContext;
};

#endif
