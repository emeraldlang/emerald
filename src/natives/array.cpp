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

#include <algorithm>

#include "emerald/execution_context.h"
#include "emerald/interpreter.h"
#include "emerald/natives/array.h"
#include "emerald/natives/utils.h"
#include "emerald/magic_methods.h"

namespace emerald {
namespace natives {

#define X(name)                                                             \
    NativeFunction* get_##name() {                                          \
        static NativeFunction func = NativeFunction(std::function(&name));  \
        return &func;                                                       \
    }
    ARRAY_NATIVES
#undef X

    NATIVE_FUNCTION(array_eq) {
        EXPECT_NUM_ARGS(2);

        TRY_CONVERT_RECV_TO(Array, lhs);
        TRY_CONVERT_ARG_TO(1, Array, rhs);

        return BOOLEAN(std::equal(
            std::begin(lhs->get_value()),
            std::end(lhs->get_value()),
            std::end(rhs->get_value()), 
            [&context](Object* lhs, Object* rhs) {
                std::vector<Object*> args({ lhs, rhs });
                return Interpreter::execute_method(magic_methods::eq, args, context)->as_bool();
            }));
    }

    NATIVE_FUNCTION(array_neq) {
        EXPECT_NUM_ARGS(2);

        TRY_CONVERT_RECV_TO(Array, lhs);
        TRY_CONVERT_ARG_TO(1, Array, rhs);

        return BOOLEAN(lhs->get_value() != rhs->get_value());
    }

    NATIVE_FUNCTION(array_clone) {
        EXPECT_NUM_ARGS(1);

        TRY_CONVERT_RECV_TO(Array, self);

        return context.get_heap().allocate<Array>(self);
    }

    NATIVE_FUNCTION(array_at) {
        EXPECT_NUM_ARGS(2);

        TRY_CONVERT_RECV_TO(Array, arr);
        TRY_CONVERT_ARG_TO(1, Number, index);

        return arr->get_value().at((long)index);
    }

    NATIVE_FUNCTION(array_front) {
        EXPECT_NUM_ARGS(1);

        TRY_CONVERT_RECV_TO(Array, arr);

        return arr->get_value().front();
    }

    NATIVE_FUNCTION(array_back) {
        EXPECT_NUM_ARGS(1);

        TRY_CONVERT_RECV_TO(Array, arr);

        return arr->get_value().back();
    }

    NATIVE_FUNCTION(array_empty) {
        EXPECT_NUM_ARGS(1);

        TRY_CONVERT_RECV_TO(Array, arr);

        return BOOLEAN(arr->get_value().empty());
    }

    NATIVE_FUNCTION(array_size) {
        EXPECT_NUM_ARGS(1);

        TRY_CONVERT_RECV_TO(Array, arr);

        return ALLOC_NUMBER(arr->get_value().size());
    }

    NATIVE_FUNCTION(array_clear) {
        EXPECT_NUM_ARGS(1);

        TRY_CONVERT_RECV_TO(Array, arr);

        arr->get_value().clear();

        return arr;
    }

    NATIVE_FUNCTION(array_push) {
        EXPECT_ATLEAST_NUM_ARGS(1);

        TRY_CONVERT_RECV_TO(Array, arr);

        std::vector<Object*>& val = arr->get_value();
        for (size_t i = 1; i < args.size(); i++) {
            val.push_back(args[i]);
        }

        return ALLOC_NUMBER(val.size());
    }

    NATIVE_FUNCTION(array_pop) {
        EXPECT_NUM_ARGS(1);

        TRY_CONVERT_RECV_TO(Array, arr);

        std::vector<Object*> val = arr->get_value();
        Object* back = val.back();
        arr->get_value().pop_back();
        return back;
    }

} // namespace natives
} // namepsace emerald
