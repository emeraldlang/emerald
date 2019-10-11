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

#include "emerald/natives/array.h"
#include "emerald/natives/utils.h"

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
        (void)heap;

        EXPECT_NUM_ARGS(2);

        TRY_CONVERT_RECV_TO(Array, lhs);
        TRY_CONVERT_ARG_TO(1, Array, rhs);

        return native_objects->get_boolean(lhs->get_value() == rhs->get_value());
    }

    NATIVE_FUNCTION(array_neq) {
        (void)heap;

        EXPECT_NUM_ARGS(2);

        TRY_CONVERT_RECV_TO(Array, lhs);
        TRY_CONVERT_ARG_TO(1, Array, rhs);

        return native_objects->get_boolean(lhs->get_value() != rhs->get_value());
    }

    NATIVE_FUNCTION(array_clone) {
        (void)native_objects;

        EXPECT_NUM_ARGS(1);

        TRY_CONVERT_RECV_TO(Array, self);

        return heap->allocate<Array>(self);
    }

    NATIVE_FUNCTION(array_at) {
        (void)heap;
        (void)native_objects;

        EXPECT_NUM_ARGS(2);

        TRY_CONVERT_RECV_TO(Array, arr);
        TRY_CONVERT_ARG_TO(1, Number, index);

        return arr->get_value().at((long)index);
    }

    NATIVE_FUNCTION(array_front) {
        (void)heap;
        (void)native_objects;

        EXPECT_NUM_ARGS(1);

        TRY_CONVERT_RECV_TO(Array, arr);

        return arr->get_value().front();
    }

    NATIVE_FUNCTION(array_back) {
        (void)heap;
        (void)native_objects;

        EXPECT_NUM_ARGS(1);

        TRY_CONVERT_RECV_TO(Array, arr);

        return arr->get_value().back();
    }

    NATIVE_FUNCTION(array_empty) {
        (void)heap;

        EXPECT_NUM_ARGS(1);

        TRY_CONVERT_RECV_TO(Array, arr);

        return native_objects->get_boolean(arr->get_value().empty());
    }

    NATIVE_FUNCTION(array_size) {
        EXPECT_NUM_ARGS(1);

        TRY_CONVERT_RECV_TO(Array, arr);

        return heap->allocate<Number>(
            native_objects->get_number_prototype(),
            arr->get_value().size());
    }

    NATIVE_FUNCTION(array_clear) {
        (void)native_objects;

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

        return heap->allocate<Number>(
            native_objects->get_number_prototype(),
            val.size());
    }

    NATIVE_FUNCTION(array_pop) {
        (void)heap;
        (void)native_objects;

        EXPECT_NUM_ARGS(1);

        TRY_CONVERT_RECV_TO(Array, arr);

        std::vector<Object*> val = arr->get_value();
        Object* back = val.back();
        arr->get_value().pop_back();
        return back;
    }

} // namespace natives
} // namepsace emerald
