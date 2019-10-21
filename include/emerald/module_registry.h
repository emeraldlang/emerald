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

#ifndef _EMERALD_MODULE_REGISTRY_H
#define _EMERALD_MODULE_REGISTRY_H

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#include "emerald/heap.h"
#include "emerald/heap_root_source.h"

#define MODULE_INITIALIZATION_FUNC(name) Module* name(ExecutionContext* context)

namespace emerald {

    class ExecutionContext;
    class Module;

    class NativeModuleInitRegistry {
    public:
        using ModuleInitialization = std::function<Module*(ExecutionContext*)>;

        static void add_module_init(const std::string& alias, ModuleInitialization initialization);
        static bool has_module_init(const std::string& alias);
        static Module* init_module(const std::string& alias, ExecutionContext* context);

    private:
        static std::unordered_map<std::string, ModuleInitialization> _modules;
    };

    class ModuleRegistry : public HeapRootSource {
    public:
        ModuleRegistry() = default;

        void add_module(Module* module);
        bool has_module(const std::string& name) const;
        const Module* get_module(const std::string& name) const;
        Module* get_module(const std::string& name);

        std::vector<HeapManaged*> get_roots() override;

    private:
        std::unordered_map<std::string, Module*> _modules;
    };

} // namespace emerald

#endif // _EMERALD_MODULE_REGISTRY_H
