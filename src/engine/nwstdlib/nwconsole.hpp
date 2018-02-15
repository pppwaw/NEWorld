// 
// nwcore: nwconsole.hpp
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

namespace LColorFunc {
    typedef std::ostream&(*colorfunc)(std::ostream& s);

#ifdef NEWORLD_TARGET_WINDOWS
    // Microsoft Windows
    static HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

    inline std::ostream& black(std::ostream& s) {
        SetConsoleTextAttribute(hStdout, 0);
        return s;
    }

    inline std::ostream& lblack(std::ostream& s) {
        SetConsoleTextAttribute(hStdout,FOREGROUND_INTENSITY);
        return s;
    }

    inline std::ostream& red(std::ostream& s) {
        SetConsoleTextAttribute(hStdout,FOREGROUND_RED);
        return s;
    }

    inline std::ostream& lred(std::ostream& s) {
        SetConsoleTextAttribute(hStdout,FOREGROUND_RED | FOREGROUND_INTENSITY);
        return s;
    }

    inline std::ostream& green(std::ostream& s) {
        SetConsoleTextAttribute(hStdout,FOREGROUND_GREEN);
        return s;
    }

    inline std::ostream& lgreen(std::ostream& s) {
        SetConsoleTextAttribute(hStdout,FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        return s;
    }

    inline std::ostream& blue(std::ostream& s) {
        SetConsoleTextAttribute(hStdout,FOREGROUND_BLUE);
        return s;
    }

    inline std::ostream& lblue(std::ostream& s) {
        SetConsoleTextAttribute(hStdout,FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        return s;
    }

    inline std::ostream& yellow(std::ostream& s) {
        SetConsoleTextAttribute(hStdout,FOREGROUND_RED | FOREGROUND_GREEN);
        return s;
    }

    inline std::ostream& lyellow(std::ostream& s) {
        SetConsoleTextAttribute(hStdout,FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        return s;
    }

    inline std::ostream& magenta(std::ostream& s) {
        SetConsoleTextAttribute(hStdout,FOREGROUND_RED | FOREGROUND_BLUE);
        return s;
    }

    inline std::ostream& lmagenta(std::ostream& s) {
        SetConsoleTextAttribute(hStdout,FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        return s;
    }

    inline std::ostream& cyan(std::ostream& s) {
        SetConsoleTextAttribute(hStdout,FOREGROUND_GREEN | FOREGROUND_BLUE);
        return s;
    }

    inline std::ostream& lcyan(std::ostream& s) {
        SetConsoleTextAttribute(hStdout,FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        return s;
    }

    inline std::ostream& white(std::ostream& s) {
        SetConsoleTextAttribute(hStdout,FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        return s;
    }

    inline std::ostream& lwhite(std::ostream& s) {
        SetConsoleTextAttribute(hStdout,FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        return s;
    }
#else
    // *nix
    inline std::ostream& black(std::ostream &s)
    { return s << "\033[21;30m"; }

    inline std::ostream& lblack(std::ostream &s)
    { return s << "\033[1;30m"; }

    inline std::ostream& red(std::ostream &s)
    { return s << "\033[21;31m"; }

    inline std::ostream& lred(std::ostream &s)
    { return s << "\033[1;31m"; }

    inline std::ostream& green(std::ostream &s)
    { return s << "\033[21;32m"; }

    inline std::ostream& lgreen(std::ostream &s)
    { return s << "\033[1;32m"; }

    inline std::ostream& blue(std::ostream &s)
    { return s << "\033[21;34m"; }

    inline std::ostream& lblue(std::ostream &s)
    { return s << "\033[1;34m"; }

    inline std::ostream& yellow(std::ostream &s)
    { return s << "\033[21;33m"; }

    inline std::ostream& lyellow(std::ostream &s)
    { return s << "\033[1;33m"; }

    inline std::ostream& magenta(std::ostream &s)
    { return s << "\033[21;35m"; }

    inline std::ostream& lmagenta(std::ostream &s)
    { return s << "\033[1;35m"; }

    inline std::ostream& cyan(std::ostream &s)
    { return s << "\033[21;36m"; }

    inline std::ostream& lcyan(std::ostream &s)
    { return s << "\033[1;36m"; }

    inline std::ostream& white(std::ostream &s)
    { return s << "\033[21;37m"; }

    inline std::ostream& lwhite(std::ostream &s)
    { return s << "\033[1;37m"; }
#endif
}

namespace LColor {
    constexpr const char* black = "&0";
    constexpr const char* red = "&1";
    constexpr const char* yellow = "&2";
    constexpr const char* green = "&3";
    constexpr const char* cyan = "&4";
    constexpr const char* blue = "&5";
    constexpr const char* magenta = "&6";
    constexpr const char* white = "&7";
    constexpr const char* lblack = "&8";
    constexpr const char* lred = "&9";
    constexpr const char* lyellow = "&a";
    constexpr const char* lgreen = "&b";
    constexpr const char* lcyan = "&c";
    constexpr const char* lblue = "&d";
    constexpr const char* lmagenta = "&e";
    constexpr const char* lwhite = "&f";
}
