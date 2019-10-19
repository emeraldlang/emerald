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

#include "emerald/module_registry.h"
#include "emerald/execution_context.h"
#include "emerald/module.h"
#include "emerald/native_objects.h"

namespace emerald {

    std::unordered_map<std::string, NativeModuleInitRegistry::ModuleInitialization> NativeModuleInitRegistry::_modules;

    void NativeModuleInitRegistry::add_module_init(const std::string& alias, ModuleInitialization initialization) {
        _modules[alias] = initialization;
    }

    bool NativeModuleInitRegistry::has_module_init(const std::string& alias) {
        return _modules.find(alias) != _modules.end();
    }

    Module* NativeModuleInitRegistry::init_module(const std::string& alias, ExecutionContext* context) {
        ModuleInitialization initialization = _modules.at(alias);
        return initialization(context);
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

    std::vector<HeapManaged*> ModuleRegistry::get_roots() {
        std::vector<HeapManaged*> roots;
        for (std::pair<std::string, Module*> pair : _modules) {
            roots.push_back(pair.second);
        }

        return roots;
    }

} // namespace emerald
