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

#include "emerald/execution_context.h"
#include "emerald/interpreter.h"
#include "emerald/natives/array.h"
#include "emerald/magic_methods.h"
#include "emerald/objectutils.h"

namespace emerald {
namespace natives {

    NATIVE_FUNCTION(array_eq) {
        EXPECT_NUM_ARGS(2);

        CONVERT_RECV_TO(Array, self);

        if (TRY_CONVERT_ARG_TO(1, Array, other)) {
            return BOOLEAN(*self == *other);
        }

        return BOOLEAN(false);
    }

    NATIVE_FUNCTION(array_neq) {
        EXPECT_NUM_ARGS(2);

        CONVERT_RECV_TO(Array, self);

        if (TRY_CONVERT_ARG_TO(1, Array, other)) {
            return BOOLEAN(*self != *other);
        }

        return BOOLEAN(true);
    }

    NATIVE_FUNCTION(array_clone) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(Array, self);

        return context->get_heap().allocate<Array>(context, self);
    }

    NATIVE_FUNCTION(array_at) {
        EXPECT_NUM_ARGS(2);

        CONVERT_RECV_TO(Array, arr);
        CONVERT_ARG_TO(1, Number, index);

        return arr->at((long)index->get_value());
    }

    NATIVE_FUNCTION(array_front) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(Array, arr);

        return arr->front();
    }

    NATIVE_FUNCTION(array_back) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(Array, arr);

        return arr->back();
    }

    NATIVE_FUNCTION(array_empty) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(Array, arr);

        return BOOLEAN(arr->empty());
    }

    NATIVE_FUNCTION(array_size) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(Array, arr);

        return ALLOC_NUMBER(arr->size());
    }

    NATIVE_FUNCTION(array_clear) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(Array, arr);

        arr->clear();

        return NONE;
    }

    NATIVE_FUNCTION(array_push) {
        EXPECT_ATLEAST_NUM_ARGS(1);

        CONVERT_RECV_TO(Array, arr);

        size_t n = args.size();
        for (size_t i = 1; i < n; i++) {
            arr->push(args[i]);
        }

        return ALLOC_NUMBER(arr->size());
    }

    NATIVE_FUNCTION(array_pop) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(Array, arr);

        return arr->pop();
    }

    NATIVE_FUNCTION(array_join) {
        EXPECT_NUM_ARGS(2);

        CONVERT_RECV_TO(Array, arr);
        CONVERT_ARG_TO(1, String, seperator);

        return ALLOC_STRING(arr->join(seperator->get_value()));
    }

} // namespace natives
} // namepsace emerald
