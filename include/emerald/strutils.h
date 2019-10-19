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

#ifndef _EMERALD_STRUTILS_H
#define _EMERALD_STRUTILS_H

#include <string>
#include <vector>

namespace emerald {
namespace strutils {

    template <class InputIt, class Transform>
    inline std::string join(InputIt first, InputIt last, const std::string& seperator, Transform transform) {
        std::string res;
        while (first != last) {
            res += transform(*first);
            if (++InputIt(first) != last) res += seperator;
            ++first;
        }

        return res;
    }

    template <class InputIt>
    inline std::string join(InputIt first, InputIt last, const std::string& seperator) {
        return join(first, last, seperator, [](const auto& v) { return v; });
    }

    inline std::vector<std::string> split(const std::string& str, const std::string& delimiters) {
        std::vector<std::string> res;
        std::string::size_type cur = str.find_first_of(delimiters), prev = 0;
        while (cur != std::string::npos) {
            res.push_back(str.substr(prev, cur - prev));
            prev = cur + 1;
            cur = str.find_first_of(delimiters, prev);
        }

        res.push_back(str.substr(prev, cur - prev));

        return res;
    }

} // namespace strutils
} // namespace emerald

#endif // _EMERALD_STRUTILS_H
