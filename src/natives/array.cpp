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
#include "emerald/magic_methods.h"
#include "emerald/natives/array.h"
#include "emerald/native_frame.h"
#include "emerald/objectutils.h"

namespace emerald {
namespace natives {

    NATIVE_FUNCTION(array_eq) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(Array, self);

        if (TRY_CONVERT_ARG_TO(0, Array, other)) {
            return BOOLEAN(*self == *other);
        }

        return BOOLEAN(false);
    }

    NATIVE_FUNCTION(array_neq) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(Array, self);

        if (TRY_CONVERT_ARG_TO(0, Array, other)) {
            return BOOLEAN(*self != *other);
        }

        return BOOLEAN(true);
    }

    NATIVE_FUNCTION(array_iter) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(Array, self);

        return Interpreter::create_obj<Array::Iterator>(
            context->get_native_objects().get_array_iterator_prototype(),
            { self },
            context);
    }

    NATIVE_FUNCTION(array_clone) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(Array, self);

        return context->get_heap().allocate<Array>(context, self);
    }

    NATIVE_FUNCTION(array_init) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(Array, self);
        self->init(frame->get_arg(0));

        return NONE;
    }

    NATIVE_FUNCTION(array_at) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(Array, self);
        CONVERT_ARG_TO(0, Number, index);

        return self->at((long)index->get_value());
    }

    NATIVE_FUNCTION(array_front) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(Array, self);

        return self->front();
    }

    NATIVE_FUNCTION(array_back) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(Array, self);

        return self->back();
    }

    NATIVE_FUNCTION(array_empty) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(Array, self);

        return self->empty();
    }

    NATIVE_FUNCTION(array_size) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(Array, self);

        return self->size();
    }

    NATIVE_FUNCTION(array_clear) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(Array, self);

        self->clear();

        return NONE;
    }

    NATIVE_FUNCTION(array_push) {
        EXPECT_ATLEAST_NUM_ARGS(1);

        CONVERT_RECV_TO(Array, self);

        size_t n = frame->num_args();
        for (size_t i = 0; i < n; i++) {
            self->push(frame->get_arg(i));
        }

        return self->size();
    }

    NATIVE_FUNCTION(array_pop) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(Array, self);

        return self->pop();
    }

    NATIVE_FUNCTION(array_join) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(Array, self);
        CONVERT_ARG_TO(0, String, seperator);

        return self->join(seperator);
    }

    NATIVE_FUNCTION(array_iterator_cur) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(Array::Iterator, self);

        return self->cur();
    }

    NATIVE_FUNCTION(array_iterator_done) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(Array::Iterator, self);

        return self->done();
    }

    NATIVE_FUNCTION(array_iterator_next) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(Array::Iterator, self);

        return self->next();
    }

    NATIVE_FUNCTION(array_iterator_clone) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(Array::Iterator, self);

        return context->get_heap().allocate<Array::Iterator>(context, self);
    }

    NATIVE_FUNCTION(array_iterator_init) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(Array::Iterator, self);
        CONVERT_ARG_TO(0, Array, arr);

        self->init(arr);

        return NONE;
    }

} // namespace natives
} // namepsace emerald
