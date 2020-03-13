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

#include <iostream>

#include "emerald/interpreter.h"
#include "emerald/magic_methods.h"
#include "emerald/module.h"
#include "emerald/modules/core.h"
#include "emerald/native_variables.h"
#include "emerald/objectutils.h"
#include "emerald/process.h"

namespace emerald {
namespace modules {

    NATIVE_FUNCTION(core_extend) {
        EXPECT_ATLEAST_NUM_ARGS(1);

        Object* target = frame->get_arg(0);
        size_t n = frame->num_args();
        for (size_t i = 1; i < n; i++) {
            for (const auto& pair : frame->get_arg(i)->get_properties()) {
                target->set_property(pair.first, pair.second);
            }
        }

        return target;
    }

    NATIVE_FUNCTION(core_str) {
        EXPECT_NUM_ARGS(1);

        return Interpreter::execute_method<String>(frame->get_arg(0), magic_methods::str, {}, process);
    }

    NATIVE_FUNCTION(core_bool) {
        EXPECT_NUM_ARGS(1);

        return Interpreter::execute_method<Boolean>(frame->get_arg(0), magic_methods::boolean, {}, process);
    }

    NATIVE_FUNCTION(core_range) {
        EXPECT_NUM_ARGS(1);

        CONVERT_ARG_TO(0, Number, n);
        Local<Array> res = ALLOC_EMPTY_ARRAY();
        for (size_t i = 0; i < n->get_native_value(); i++) {
            res->push(ALLOC_NUMBER(i));
        }

        return res.val();
    }

    NATIVE_FUNCTION(core_super) {
        EXPECT_NUM_ARGS(1);

        if (Object* parent = frame->get_arg(0)->get_parent()) {
            return parent;
        }

        return NONE;
    }

    NATIVE_FUNCTION(core_iter) {
        EXPECT_NUM_ARGS(1);

        return Interpreter::execute_method<Object>(frame->get_arg(0), magic_methods::iter, {}, process);
    }

    NATIVE_FUNCTION(core_cur) {
        EXPECT_NUM_ARGS(1);

        return Interpreter::execute_method<Object>(frame->get_arg(0), magic_methods::cur, {}, process);
    }

    NATIVE_FUNCTION(core_done) {
        EXPECT_NUM_ARGS(1);

        return Interpreter::execute_method<Boolean>(frame->get_arg(0), magic_methods::done, {}, process);
    }

    NATIVE_FUNCTION(core_next) {
        EXPECT_NUM_ARGS(1);

        return Interpreter::execute_method<Object>(frame->get_arg(0), magic_methods::next, {}, process);
    }

    NATIVE_FUNCTION(core_print) {
        size_t n = frame->num_args();
        for (size_t i = 0; i < n; i++) {
            std::cout << Interpreter::execute_method<String>(frame->get_arg(i), magic_methods::str, {}, process)->get_native_value() << std::endl;
        }

        return NONE;
    }

    MODULE_INITIALIZATION_FUNC(init_core_module) {
        Process* process =  module->get_process();

        module->set_property("Array", ARRAY_PROTOTYPE);
        module->set_property("Boolean", BOOLEAN_PROTOTYPE);
        module->set_property("Exception", EXCEPTION_PROTOTYPE);
        module->set_property("Number", NUMBER_PROTOTYPE);
        module->set_property("Object", OBJECT_PROTOTYPE);
        module->set_property("String", STRING_PROTOTYPE);

        module->set_property("extend", ALLOC_NATIVE_FUNCTION(core_extend));
        module->set_property("str", ALLOC_NATIVE_FUNCTION(core_str));
        module->set_property("bool", ALLOC_NATIVE_FUNCTION(core_bool));
        module->set_property("range", ALLOC_NATIVE_FUNCTION(core_range));
        module->set_property("super", ALLOC_NATIVE_FUNCTION(core_super));
        module->set_property("iter", ALLOC_NATIVE_FUNCTION(core_iter));
        module->set_property("cur", ALLOC_NATIVE_FUNCTION(core_cur));
        module->set_property("done", ALLOC_NATIVE_FUNCTION(core_done));
        module->set_property("next", ALLOC_NATIVE_FUNCTION(core_next));
        module->set_property("print", ALLOC_NATIVE_FUNCTION(core_print));
    }

} // namespace modules
} // namespace emerald
