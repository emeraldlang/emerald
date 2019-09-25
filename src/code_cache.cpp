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

#include <filesystem>
#include <string>

#include "emerald/code_cache.h"
#include "emerald/strutils.h"

namespace emerald {

    ConcurrentMap<std::string, std::shared_ptr<Code>> CodeCache::_code;

    std::shared_ptr<Code> CodeCache::get_code(const std::string& module_name) {
        if (_code.has(module_name)) {
            return _code.get(module_name);
        }

        return nullptr;
    }

    std::shared_ptr<Code> CodeCache::get_or_load_code(const std::string& module_name) {
        if (std::shared_ptr<Code> code = get_code(module_name)) {
            return code;
        }

        load_code(module_name);
        return _code.get(module_name);
    }

    void CodeCache::load_code(const std::string& module_name) {
        std::filesystem::path path = std::filesystem::current_path();
        for (const std::string& part : strutils::split(module_name, ".")) {
            path /= part;
        }
        path += ".emc";

        std::shared_ptr<Code> code = Code::from_file(path);
        for (const std::string& import_name : code->get_import_names()) {
            if (_code.has(import_name)) continue;
            load_code(import_name);
        }

        _code[module_name] = code;
    }

} // namespace emerald
