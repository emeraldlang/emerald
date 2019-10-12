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

#include <functional>

#include "emerald/natives/number.h"
#include "emerald/natives/utils.h"

namespace emerald {
namespace natives {

#define X(name)                                                             \
    NativeFunction* get_##name() {                                          \
        static NativeFunction func = NativeFunction(std::function(&name));  \
        return &func;                                                       \
    }
    NUMBER_NATIVES
#undef X

    NATIVE_FUNCTION(number_neg) {
        EXPECT_NUM_ARGS(1);

        TRY_CONVERT_RECV_TO(Number, val);

        return ALLOC_NUMBER(-val->get_value());
    }

    NATIVE_FUNCTION(number_add) {
        EXPECT_NUM_ARGS(2);

        TRY_CONVERT_RECV_TO(Number, lhs);
        TRY_CONVERT_ARG_TO(1, Number, rhs);

        return ALLOC_NUMBER(lhs->get_value() + rhs->get_value());
    }

    NATIVE_FUNCTION(number_sub) {
        EXPECT_NUM_ARGS(2);

        TRY_CONVERT_RECV_TO(Number, lhs);
        TRY_CONVERT_ARG_TO(1, Number, rhs);

        return ALLOC_NUMBER(lhs->get_value() - rhs->get_value());
    }

    NATIVE_FUNCTION(number_mul) {
        EXPECT_NUM_ARGS(2);

        TRY_CONVERT_RECV_TO(Number, lhs);
        TRY_CONVERT_ARG_TO(1, Number, rhs);

        return ALLOC_NUMBER(lhs->get_value() * rhs->get_value());
    }

    NATIVE_FUNCTION(number_div) {
        EXPECT_NUM_ARGS(2);

        TRY_CONVERT_RECV_TO(Number, lhs);
        TRY_CONVERT_ARG_TO(1, Number, rhs);

        return ALLOC_NUMBER(lhs->get_value() / rhs->get_value());
    }

    NATIVE_FUNCTION(number_mod) {
        EXPECT_NUM_ARGS(2);

        TRY_CONVERT_RECV_TO(Number, lhs);
        TRY_CONVERT_ARG_TO(1, Number, rhs);

        return ALLOC_NUMBER((long)lhs->get_value() % (long)rhs->get_value());
    }

    NATIVE_FUNCTION(number_iadd) {
        (void)heap;
        (void)native_objects;
        
        EXPECT_NUM_ARGS(2);

        TRY_CONVERT_RECV_TO(Number, lhs);
        TRY_CONVERT_ARG_TO(1, Number, rhs);

        lhs->set_value(lhs->get_value() + rhs->get_value());

        return lhs;
    }

    NATIVE_FUNCTION(number_isub) {
        (void)heap;
        (void)native_objects;
        
        EXPECT_NUM_ARGS(2);

        TRY_CONVERT_RECV_TO(Number, lhs);
        TRY_CONVERT_ARG_TO(1, Number, rhs);

        lhs->set_value(lhs->get_value() - rhs->get_value());

        return lhs;
    }

    NATIVE_FUNCTION(number_imul) {
        (void)heap;
        (void)native_objects;
        
        EXPECT_NUM_ARGS(2);

        TRY_CONVERT_RECV_TO(Number, lhs);
        TRY_CONVERT_ARG_TO(1, Number, rhs);

        lhs->set_value(lhs->get_value() * rhs->get_value());

        return lhs;
    }

    NATIVE_FUNCTION(number_idiv) {
        (void)heap;
        (void)native_objects;
        
        EXPECT_NUM_ARGS(2);

        TRY_CONVERT_RECV_TO(Number, lhs);
        TRY_CONVERT_ARG_TO(1, Number, rhs);

        lhs->set_value(lhs->get_value() / rhs->get_value());

        return lhs;
    }

    NATIVE_FUNCTION(number_imod) {
        (void)heap;
        (void)native_objects;
        
        EXPECT_NUM_ARGS(2);

        TRY_CONVERT_RECV_TO(Number, lhs);
        TRY_CONVERT_ARG_TO(1, Number, rhs);

        lhs->set_value((long)lhs->get_value() % (long)rhs->get_value());

        return lhs;
    }

    NATIVE_FUNCTION(number_eq) {
        (void)heap;

        EXPECT_NUM_ARGS(2);

        TRY_CONVERT_RECV_TO(Number, lhs);
        TRY_CONVERT_ARG_TO(1, Number, rhs);

        return BOOLEAN(lhs->get_value() == rhs->get_value());
    }

    NATIVE_FUNCTION(number_neq) {
        (void)heap;

        EXPECT_NUM_ARGS(2);

        TRY_CONVERT_RECV_TO(Number, lhs);
        TRY_CONVERT_ARG_TO(1, Number, rhs);

        return BOOLEAN(lhs->get_value() != rhs->get_value());
    }

    NATIVE_FUNCTION(number_lt) {
        (void)heap;

        EXPECT_NUM_ARGS(2);

        TRY_CONVERT_RECV_TO(Number, lhs);
        TRY_CONVERT_ARG_TO(1, Number, rhs);

        return BOOLEAN(lhs->get_value() < rhs->get_value());
    }

    NATIVE_FUNCTION(number_gt) {
        (void)heap;

        EXPECT_NUM_ARGS(2);

        TRY_CONVERT_RECV_TO(Number, lhs);
        TRY_CONVERT_ARG_TO(1, Number, rhs);

        return BOOLEAN(lhs->get_value() > rhs->get_value());
    }

    NATIVE_FUNCTION(number_lte) {
        (void)heap;

        EXPECT_NUM_ARGS(2);

        TRY_CONVERT_RECV_TO(Number, lhs);
        TRY_CONVERT_ARG_TO(1, Number, rhs);

        return BOOLEAN(lhs->get_value() <= rhs->get_value());
    }

    NATIVE_FUNCTION(number_gte) {
        (void)heap;

        EXPECT_NUM_ARGS(2);

        TRY_CONVERT_RECV_TO(Number, lhs);
        TRY_CONVERT_ARG_TO(1, Number, rhs);

        return BOOLEAN(lhs->get_value() >= rhs->get_value());
    }

    NATIVE_FUNCTION(number_bit_or) {
        EXPECT_NUM_ARGS(2);

        TRY_CONVERT_RECV_TO(Number, lhs);
        TRY_CONVERT_ARG_TO(1, Number, rhs);

        return ALLOC_NUMBER((long)lhs->get_value() | (long)rhs->get_value());
    }

    NATIVE_FUNCTION(number_bit_xor) {
        EXPECT_NUM_ARGS(2);

        TRY_CONVERT_RECV_TO(Number, lhs);
        TRY_CONVERT_ARG_TO(1, Number, rhs);

        return ALLOC_NUMBER((long)lhs->get_value() ^ (long)rhs->get_value());
    }

    NATIVE_FUNCTION(number_bit_and) {
        EXPECT_NUM_ARGS(2);

        TRY_CONVERT_RECV_TO(Number, lhs);
        TRY_CONVERT_ARG_TO(1, Number, rhs);

        return ALLOC_NUMBER((long)lhs->get_value() & (long)rhs->get_value());
    }

    NATIVE_FUNCTION(number_bit_shl) {
        EXPECT_NUM_ARGS(2);

        TRY_CONVERT_RECV_TO(Number, lhs);
        TRY_CONVERT_ARG_TO(1, Number, rhs);

        return ALLOC_NUMBER((long)lhs->get_value() << (long)rhs->get_value());
    }

    NATIVE_FUNCTION(number_bit_shr) {
        EXPECT_NUM_ARGS(2);

        TRY_CONVERT_RECV_TO(Number, lhs);
        TRY_CONVERT_ARG_TO(1, Number, rhs);

        return ALLOC_NUMBER((long)lhs->get_value() >> (long)rhs->get_value());
    }

    NATIVE_FUNCTION(number_clone) {
        (void)native_objects;

        EXPECT_NUM_ARGS(1);

        TRY_CONVERT_RECV_TO(Number, self);

        return heap->allocate<Number>(self);
    }

} // namespace natives
} // namespace emerald
