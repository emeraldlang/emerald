/*  Emerald - Procedural and Object Oriented Programming Language
**  Copyright (C) 2018  Zach Perkitny
**
**  This program is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef _EMERALD_COLORS_H
#define _EMERALD_COLORS_H

#include <ostream>

#ifdef _WIN32
#include <windows.h>
#endif

namespace emerald {
namespace colors {

    inline std::ostream& reset(std::ostream& stream) {
#ifdef _WIN32
        // TODO(zvp): Windows colors
#else
        stream << "\033[0m";
#endif
        return stream;
    }

    inline std::ostream& bold(std::ostream& stream) {
#ifdef _WIN32
        // TODO(zvp): Windows colors
#else
        stream << "\033[1m";
#endif
        return stream;
    }

    inline std::ostream& underline(std::ostream& stream) {
#ifdef _WIN32
        // TODO(zvp): Windows colors
#else
        stream << "\033[4m";
#endif
        return stream;
    }

    inline std::ostream& bg_default(std::ostream& stream) {
#ifdef _WIN32
        setWindowsConsoleAttributes(stream, -1);
#else
        stream << "\033[49m";
#endif
        return stream;
    }

    inline std::ostream& fg_default(std::ostream& stream) {
#ifdef _WIN32
        setWindowsConsoleAttributes(stream, -1);
#else
        stream << "\033[39m";
#endif
        return stream;
    }

    inline std::ostream& bg_red(std::ostream& stream) {
#ifdef _WIN32
        // TODO(zvp): Windows colors
#else
        stream << "\033[41m";
#endif
        return stream;
    }

    inline std::ostream& fg_red(std::ostream& stream) {
#ifdef _WIN32
        // TODO(zvp): Windows colors
#else
        stream << "\033[31m";
#endif
        return stream;
    }

    inline std::ostream& bg_green(std::ostream& stream) {
#ifdef _WIN32
        // TODO(zvp): Windows colors
#else
        stream << "\033[42m";
#endif
        return stream;
    }

    inline std::ostream& fg_green(std::ostream& stream) {
#ifdef _WIN32
        // TODO(zvp): Windows colors
#else
        stream << "\033[32m";
#endif
        return stream;
    }

    inline std::ostream& bg_yellow(std::ostream& stream) {
#ifdef _WIN32
        // TODO(zvp): Windows colors
#else
        stream << "\033[43m";
#endif
        return stream;
    }

    inline std::ostream& fg_yellow(std::ostream& stream) {
#ifdef _WIN32
        // TODO(zvp): Windows colors
#else
        stream << "\033[33m";
#endif
        return stream;
    }

    inline std::ostream& bg_blue(std::ostream& stream) {
#ifdef _WIN32
        // TODO(zvp): Windows colors
#else
        stream << "\033[44m";
#endif
        return stream;
    }

    inline std::ostream& fg_blue(std::ostream& stream) {
#ifdef _WIN32
        // TODO(zvp): Windows colors
#else
        stream << "\033[34m";
#endif
        return stream;
    }

    inline std::ostream& bg_magenta(std::ostream& stream) {
#ifdef _WIN32
        // TODO(zvp): Windows colors
#else
        stream << "\033[45m";
#endif
        return stream;
    }

    inline std::ostream& fg_magenta(std::ostream& stream) {
#ifdef _WIN32
        // TODO(zvp): Windows colors
#else
        stream << "\033[35m";
#endif
        return stream;
    }

    inline std::ostream& bg_cyan(std::ostream& stream) {
#ifdef _WIN32
        // TODO(zvp): Windows colors
#else
        stream << "\033[46m";
#endif
        return stream;
    }

    inline std::ostream& fg_cyan(std::ostream& stream) {
#ifdef _WIN32
        // TODO(zvp): Windows colors
#else
        stream << "\033[36m";
#endif
        return stream;
    }

    inline std::ostream& bg_white(std::ostream& stream) {
#ifdef _WIN32
        // TODO(zvp): Windows colors
#else
        stream << "\033[47m";
#endif
        return stream;
    }

    inline std::ostream& fg_white(std::ostream& stream) {
#ifdef _WIN32
        // TODO(zvp): Windows colors
#else
        stream << "\033[37m";
#endif
        return stream;
    }

#ifdef _WIN32
    static void setWindowsConsoleAttributes(std::ostream& stream, WORD wAttributes) {
        static WORD wDefaultAttributes = -1;

        HANDLE hConsoleOutput = INVALID_HANDLE_VALUE;
        if (stream == &std::cout) {
            hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
        } else if (stream == &std::cerr) {
            hConsoleOutput = GetStdHandle(STD_ERROR_HANDLE);
        }

        if (wDefaultAttributes == -1) {
            CONSOLE_SCREEN_BUFFER_INFO csBufferInfo;
            if (GetConsoleScreenBufferInfo(hConsoleOutput, &csBufferInfo)) {
                wDefaultAttributes = csBufferInfo.wAttributes;
            }
        }

        if (wAttributes == -1) {
            SetConsoleTextAttribute(hConsoleOutput, wDefaultAttributes);
        } else {
            SetConsoleTextAttribute(hConsoleOutput, wAttributes);
        }
    } 
#endif

} // namespace colors
} // namespace emerald

#endif // _EMERALD_COLORS_H
