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

#ifndef _EMERALD_CODE_CACHE_H
#define _EMERALD_CODE_CACHE_H

#include <string>
#include <unordered_map>

#include "emerald/code.h"

namespace emerald {

    class CodeCache {
    public:
        static std::shared_ptr<Code> get_code(const std::string& module_name);
        static std::shared_ptr<Code> get_or_load_code(const std::string& module_name);

    private:
        static std::unordered_map<std::string, std::shared_ptr<Code>> _code;

        static void load_code(const std::string& module_name);
        static std::filesystem::path locate_code(const std::string& module_name);
    };

} // namespace emerald

#endif // _EMERALD_CODE_CACHE_H
