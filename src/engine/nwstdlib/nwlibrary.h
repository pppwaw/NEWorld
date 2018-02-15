// 
// nwcore: nwlibrary.h
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

#pragma once

#ifndef NEWORLD_TARGET_WINDOWS
    #include <dlfcn.h>
#endif

class Library : public NonCopyable {
public:
    Library() = default;

    Library(const std::string& filename) : Library{} { load(filename); }

    Library(Library&& library) noexcept : Library {} {
        std::swap(library.mDllHandle, mDllHandle);
        std::swap(library.mLoaded, mLoaded);
    }

    Library&& operator=(Library&& library) noexcept {
        std::swap(library.mDllHandle, mDllHandle);
        std::swap(library.mLoaded, mLoaded);
        return std::move(*this);
    }

    ~Library() {
        if (isLoaded())
            freeLibrary(mDllHandle);
    }

    template <class T>
    auto get(const std::string& name) { return getFunc<T>(mDllHandle, name); }

    operator bool() const { return isLoaded(); }

    bool isLoaded() const { return mLoaded; }

    void load(const std::string& filename) {
        if (isLoaded())
            unload();
        mDllHandle = loadLibrary(filename, mLoaded);
    }

    void unload() {
        freeLibrary(mDllHandle);
        mLoaded = false;
    }

private:

#ifdef NEWORLD_TARGET_WINDOWS

    using HandleType = HMODULE;

    static HandleType loadLibrary(const std::string& filename, bool& success) {
        HandleType handle = LoadLibraryA(filename.c_str());
        success = handle != nullptr;
        if (!success) {
            auto errcode = GetLastError();
            warningstream << "Failed to load " << filename << ". Error code:" << errcode;
            if (errcode == 126) {
                warningstream << "Note: Error code 126 usually means that you have dependencies missing.";
                warningstream << "      Please make sure you have placed all dependent dll files along with the game.";
            }
        }
        return handle;
    }

    template <class T>
    static auto getFunc(HandleType handle, const std::string& name) {
        Assert(handle != nullptr);
        return reinterpret_cast<std::decay_t<T>>(GetProcAddress(handle, name.c_str()));
    }

    static void freeLibrary(HandleType handle) { FreeLibrary(handle); }

#else

    using HandleType = void*;

    static HandleType loadLibrary(const std::string& filename, bool& success)
    {
        HandleType handle = dlopen(filename.c_str(), RTLD_LAZY);
        if (handle == nullptr)
            fatalstream << dlerror();
        success = handle != nullptr;
        return handle;
    }

    template<class T> static auto getFunc(HandleType handle, const std::string& name)
    {
        Assert(handle != nullptr);
        return reinterpret_cast<std::decay_t<T>>(dlsym(handle, name.c_str()));
    }

    static void freeLibrary(HandleType handle)
    {
        dlclose(handle);
    }

#endif

    HandleType mDllHandle;
    bool mLoaded = false;
};
