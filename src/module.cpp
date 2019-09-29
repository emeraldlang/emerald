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

#include "fmt/format.h"

#include "emerald/module.h"
#include "emerald/strutils.h"

namespace emerald {

    Module::Module(const std::string& name)
        : _name(name) {}

    Module::Module(const std::string& name, std::shared_ptr<Code> code)
        : _name(name),
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

    std::filesystem::path Module::get_path_for_module(
            const std::string& module_name,
            const std::string& extension) {
        std::filesystem::path path = std::filesystem::current_path();
        for (const std::string& part : strutils::split(module_name, ".")) {
            path /= part;
        }
        path += extension;

        return path;
    }

    ConcurrentMap<std::string, NativeModuleInitRegistry::ModuleInitialization> NativeModuleInitRegistry::_modules;

    void NativeModuleInitRegistry::add_module_init(const std::string& alias, ModuleInitialization initialization) {
        _modules[alias] = initialization;
    }

    bool NativeModuleInitRegistry::is_module_init_registered(const std::string& alias) {
        return _modules.has(alias);
    }

    Module* NativeModuleInitRegistry::init_module(
        const std::string& alias,
        Heap* heap,
        NativePrototypes* native_prototypes) {
        ModuleInitialization initialization = _modules.get(alias);
        return initialization(heap, native_prototypes);
    }

    void ModuleRegistry::add_module(Module* module) {
        _modules[module->get_name()] = module;
    }

    bool ModuleRegistry::has_module(const std::string& name) const {
        return _modules.find(name) != _modules.end();
    }

    const Module* ModuleRegistry::get_module(const std::string& name) const {
        return _modules.at(name);
    }
    
    Module* ModuleRegistry::get_module(const std::string& name) {
        return _modules.at(name);
    }

    std::vector<HeapManaged*> ModuleRegistry::get_roots() const {
        std::vector<HeapManaged*> roots;
        for (std::pair<std::string, Module*> pair : _modules) {
            roots.push_back(pair.second);
        }

        return roots;
    }

} // namespace emerald
