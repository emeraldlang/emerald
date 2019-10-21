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

#include "emerald/modules/core.h"
#include "emerald/execution_context.h"
#include "emerald/interpreter.h"
#include "emerald/natives/boolean.h"
#include "emerald/magic_methods.h"
#include "emerald/module.h"
#include "emerald/objectutils.h"

namespace emerald {
namespace modules {

    NATIVE_FUNCTION(core_extend) {
        EXPECT_ATLEAST_NUM_ARGS(1);

        size_t n = args.size();
        for (size_t i = 1; i < n; i++) {
            for (const auto& pair : args[i]->get_properties()) {
                args[0]->set_property(pair.first, pair.second);
            }
        }

        return args[0];
    }

    NATIVE_FUNCTION(core_str) {
        EXPECT_NUM_ARGS(1);

        return Interpreter::execute_method(magic_methods::str, args, context);
    }

    NATIVE_FUNCTION(core_bool) {
        EXPECT_NUM_ARGS(1);

        return Interpreter::execute_method(magic_methods::boolean, args, context);
    }

    NATIVE_FUNCTION(core_range) {
        EXPECT_NUM_ARGS(1);

        TRY_CONVERT_ARG_TO(0, Number, n);
        Array* res = ALLOC_EMPTY_ARRAY();
        for (size_t i = 0; i < n->get_value(); i++) {
            res->push(ALLOC_NUMBER(i));
        }

        return res;
    }

    MODULE_INITIALIZATION_FUNC(init_core_module) {
        Module* module = context->get_heap().allocate<Module>(context, "core");

        NativeObjects& native_objects = context->get_native_objects();
        module->set_property("Object", native_objects.get_object_prototype());
        module->set_property("Array", native_objects.get_array_prototype());
        module->set_property("Boolean", native_objects.get_boolean_prototype());
        module->set_property("Number", native_objects.get_number_prototype());
        module->set_property("String", native_objects.get_string_prototype());

        module->set_property("extend", ALLOC_NATIVE_FUNCTION(core_extend));
        module->set_property("str", ALLOC_NATIVE_FUNCTION(core_str));
        module->set_property("bool", ALLOC_NATIVE_FUNCTION(core_bool));
        module->set_property("range", ALLOC_NATIVE_FUNCTION(core_range));

        return module;
    }

} // namespace modules
} // namespace emerald
