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

#ifndef _EMERALD_MODULE_H
#define _EMERALD_MODULE_H

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

#include "emerald/heap.h"
#include "emerald/native_prototypes.h"
#include "emerald/object.h"

#define MODULE_INITIALIZATION_FUNC(name) std::shared_ptr<Module> name(Heap* heap, NativePrototypes* native_prototypes)

namespace emerald {

    class Module {
    public:
        Module(const std::string& name);

        const std::string& get_name() const;

        void add_object(const std::string& name, Object* obj);

        const Object* get_object(const std::string& name) const;
        Object* get_object(const std::string& name);

    private:
        std::string _name;
        std::unordered_map<std::string, Object*> _objects;
    };

    class NativeModuleRegistry {
    public:
        using ModuleInitialization = std::function<std::shared_ptr<Module>(Heap*, NativePrototypes*)>;

        static void add_module(const std::string& name, ModuleInitialization initialization);
        static bool is_module_registered(const std::string& name);
        static std::shared_ptr<Module> initialize_module(
            const std::string& name,
            Heap* heap,
            NativePrototypes* native_prototypes);

    private:
        static std::unordered_map<std::string, ModuleInitialization> _modules;
    };

} // namespace emerald

#endif // _EMERALD_MODULE_H
