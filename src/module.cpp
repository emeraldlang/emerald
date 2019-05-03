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

#include "emerald/module.h"

namespace emerald {

    Module::Module(const std::string& name)
        : _name(name) {}

    const std::string& Module::get_name() const {
        return _name;
    }

    void Module::add_object(const std::string& name, Object* obj) {
        _objects[name] = obj;
    }

    const Object* Module::get_object(const std::string& name) const {
        return _objects.at(name);
    }

    Object* Module::get_object(const std::string& name) {
        return _objects.at(name);
    }

    void NativeModuleRegistry::add_module(const std::string& name, ModuleInitialization initialization) {
        _modules[name] = initialization;
    }

    std::shared_ptr<Module> NativeModuleRegistry::initialize_module(const std::string& name, Heap* heap) {
        ModuleInitialization initialization = _modules.at(name);
        return initialization(heap);
    }

    std::unordered_map<std::string, NativeModuleRegistry::ModuleInitialization> NativeModuleRegistry::_modules = {};

} // namespace emerald
