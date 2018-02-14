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

#ifndef WIDGET_H
#define WIDGET_H
#include <functional>
#include "nuklear_helper.h"
#include <string>
#include "engine/nwmath/nwvector.h"

// widget base class
class Widget
{
public:
    Widget(nk_context* nkContext, std::string name, struct nk_rect size, int flags)
        : mName(std::move(name)), mSize(size), mNkContext(nkContext), mFlags(flags) {}
    virtual ~Widget() {}
    void _render()
    {
        if (nk_begin(mNkContext, mName.c_str(), mSize, mFlags))
            render();
        nk_menubar_end(mNkContext);
    }
    virtual void update() = 0;

    void setOpen(bool open) { mOpen = open; }
    std::string getName() const { return mName; }
protected:
    virtual void render() = 0;

private:
    std::string mName;
    bool mOpen = true;
    struct nk_rect mSize;
    nk_context* mNkContext;
    int mFlags;
};

// callback style widget
class WidgetCallback : public Widget
{
public:
    using Callback = std::function<void(void)>;
    WidgetCallback(nk_context* ctx, std::string name, struct nk_rect size, int flags,
        Callback renderFunc, Callback updateFunc = nullptr) :
        Widget(ctx, name, size, flags), mRenderFunc(renderFunc), mUpdateFunc(updateFunc) {}

    void render() override { mRenderFunc(); }
    void update() override { if(mUpdateFunc) mUpdateFunc(); }

private:
    Callback mRenderFunc, mUpdateFunc;
};

#endif
