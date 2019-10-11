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

#include <filesystem>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#include "emerald/code.h"
#include "emerald/concurrent_map.h"
#include "emerald/heap.h"
#include "emerald/heap_root_source.h"
#include "emerald/native_objects.h"
#include "emerald/object.h"

#define MODULE_INITIALIZATION_FUNC(name) Module* name(Heap* heap, NativeObjects* native_objects)

namespace emerald {

    class Module : public HeapObject {
    public:
        Module(const std::string& name);
        Module(const std::string& name, std::shared_ptr<Code> code);

        const std::string& get_name() const;
        std::shared_ptr<Code> get_code() const;

        bool is_native() const;

        std::string as_str() const override;

        static std::filesystem::path get_path_for_module(
            const std::string& module_name,
            const std::string& extension);

    private:
        std::string _name;
        std::shared_ptr<Code> _code;
    };

    class NativeModuleRegistry {
    public:
        using ModuleInitialization = std::function<Module*(Heap*, NativeObjects*)>;

        static void add_module(const std::string& alias, ModuleInitialization initialization);
        static bool has_module(const std::string& alias);
        static Module* init_module(
            const std::string& alias,
            Heap* heap,
            NativeObjects* native_objects);

    private:
        static ConcurrentMap<std::string, ModuleInitialization> _modules;
    };

    class ModuleRegistry : public HeapRootSource {
    public:
        ModuleRegistry() = default;

        void add_module(Module* module);
        bool has_module(const std::string& name) const;
        const Module* get_module(const std::string& name) const;
        Module* get_module(const std::string& name);

        std::vector<HeapManaged*> get_roots() const override;

    private:
        std::unordered_map<std::string, Module*> _modules;
    };

} // namespace emerald

#endif // _EMERALD_MODULE_H
