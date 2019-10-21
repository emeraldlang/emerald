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
#include "emerald/module.h"
#include "emerald/module_registry.h"
#include "emerald/strutils.h"

namespace emerald {

    std::unordered_map<std::string, std::shared_ptr<Code>> CodeCache::_code;

    std::shared_ptr<Code> CodeCache::get_code(const std::string& module_name) {
        if (_code.find(module_name) != _code.end()) {
            return _code.at(module_name);
        }

        return nullptr;
    }

    std::shared_ptr<Code> CodeCache::get_or_load_code(const std::string& module_name) {
        if (std::shared_ptr<Code> code = get_code(module_name)) {
            return code;
        }

        load_code(module_name);
        return _code.at(module_name);
    }

    void CodeCache::load_code(const std::string& module_name) {
        if (NativeModuleInitRegistry::has_module_init(module_name)) {
            return;
        }

        std::filesystem::path path = locate_code(module_name);
        std::shared_ptr<Code> code = std::make_shared<Code>(path);
        _code[module_name] = code;
        for (const std::string& import_name : code->get_import_names()) {
            if (_code.find(import_name) != _code.end()) continue;
            load_code(import_name);
        }
    }

    std::filesystem::path CodeCache::locate_code(const std::string& module_name) {
        std::filesystem::path module_path = Module::get_module_path(module_name, ".emc");

        std::filesystem::path possible_path = std::filesystem::current_path() / module_path;
        if (std::filesystem::is_regular_file(possible_path)) {
            return possible_path;
        }

        possible_path = Module::get_stdlib_path() / module_path;
        if (std::filesystem::is_regular_file(possible_path)) {
            return possible_path;
        }

        // ?
        throw;
    }

} // namespace emerald
