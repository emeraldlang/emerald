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

#ifdef _WIN32
#include <windows.h>
#else
#include <linux/limits.h>
#include <unistd.h>
#endif

#include "fmt/format.h"

#include "emerald/module.h"
#include "emerald/process.h"
#include "emerald/objectutils.h"
#include "emerald/strutils.h"

namespace emerald {

    Module::Module(Process* process, const std::string& name, std::shared_ptr<Code> code)
        : Object(process, OBJECT_PROTOTYPE), 
        _name(name),
        _code(code) {}

    Module::Module(Process* process, Object* parent, const std::string& name, std::shared_ptr<Code> code)
        : Object(process, parent), 
        _name(name),
        _code(code) {}

    const std::string& Module::get_name() const {
        return _name;
    }

    std::shared_ptr<Code> Module::get_code() const {
        return _code;
    }

    bool Module::is_native() const {
        return _code == nullptr;
    }

    std::string Module::as_str() const {
        return fmt::format("<module {0}>", _name);
    }

    Module* Module::clone(Process* process, CloneCache& cache) {
        return clone_impl<Module>(process, cache, _name, _code);
    }

    std::filesystem::path Module::get_module_path(
            const std::string& module_name,
            const std::string& extension) {
        std::filesystem::path path;
        for (const std::string& part : strutils::split(module_name, ".")) {
            path /= part;
        }
        path += extension;

        return path;
    }

    std::filesystem::path Module::get_stdlib_path() {
#ifdef _WIN32
        char buff[MAX_PATH];
        GetModuleFileName(nullptr, buff, MAX_PATH);   
#else
        char buff[PATH_MAX];
        readlink("/proc/self/exe", buff, PATH_MAX);
#endif
        return std::filesystem::path(buff).parent_path().parent_path() / "lib";
    }

} // namespace emerald
