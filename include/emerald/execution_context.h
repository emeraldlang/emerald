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

#ifndef _EMERALD_EXECUTION_CONTEXT_H
#define _EMERALD_EXECUTION_CONTEXT_H

#include "emerald/heap.h"
#include "emerald/module_registry.h"
#include "emerald/native_objects.h"
#include "emerald/stack.h"

namespace emerald {

    class ExecutionContext {
    public:
        ExecutionContext()
            : _native_objects(this) {
            _heap.add_root_source(&_module_registry);
            _heap.add_root_source(&_native_objects);
            _heap.add_root_source(&_stack);
        }

        const Heap& get_heap() const { return _heap; }
        Heap& get_heap() { return _heap; }

        const ModuleRegistry& get_module_registry() const { return _module_registry; }
        ModuleRegistry& get_module_registry() { return _module_registry; }

        const NativeObjects& get_native_objects() const { return _native_objects; }
        NativeObjects& get_native_objects() { return _native_objects; }

        const Stack& get_stack() const { return _stack; }
        Stack& get_stack() { return _stack; }

    private:
        Heap _heap;
        ModuleRegistry _module_registry;
        NativeObjects _native_objects;
        Stack _stack;
    };

} // namespace emerald

#endif // _EMERALD_EXECUTION_CONTEXT_H
