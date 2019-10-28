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
#include "emerald/natives/number.h"
#include "emerald/native_frame.h"
#include "emerald/objectutils.h"

namespace emerald {
namespace natives {

    NATIVE_FUNCTION(number_neg) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(Number, val);

        return ALLOC_NUMBER(-val->get_value());
    }

    NATIVE_FUNCTION(number_add) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(Number, lhs);
        CONVERT_ARG_TO(0, Number, rhs);

        return ALLOC_NUMBER(lhs->get_value() + rhs->get_value());
    }

    NATIVE_FUNCTION(number_sub) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(Number, lhs);
        CONVERT_ARG_TO(0, Number, rhs);

        return ALLOC_NUMBER(lhs->get_value() - rhs->get_value());
    }

    NATIVE_FUNCTION(number_mul) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(Number, lhs);
        CONVERT_ARG_TO(0, Number, rhs);

        return ALLOC_NUMBER(lhs->get_value() * rhs->get_value());
    }

    NATIVE_FUNCTION(number_div) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(Number, lhs);
        CONVERT_ARG_TO(0, Number, rhs);

        return ALLOC_NUMBER(lhs->get_value() / rhs->get_value());
    }

    NATIVE_FUNCTION(number_mod) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(Number, lhs);
        CONVERT_ARG_TO(0, Number, rhs);

        return ALLOC_NUMBER((long)lhs->get_value() % (long)rhs->get_value());
    }

    NATIVE_FUNCTION(number_iadd) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(Number, lhs);
        CONVERT_ARG_TO(0, Number, rhs);

        lhs->set_value(lhs->get_value() + rhs->get_value());

        return lhs;
    }

    NATIVE_FUNCTION(number_isub) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(Number, lhs);
        CONVERT_ARG_TO(0, Number, rhs);

        lhs->set_value(lhs->get_value() - rhs->get_value());

        return lhs;
    }

    NATIVE_FUNCTION(number_imul) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(Number, lhs);
        CONVERT_ARG_TO(0, Number, rhs);

        lhs->set_value(lhs->get_value() * rhs->get_value());

        return lhs;
    }

    NATIVE_FUNCTION(number_idiv) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(Number, lhs);
        CONVERT_ARG_TO(0, Number, rhs);

        lhs->set_value(lhs->get_value() / rhs->get_value());

        return lhs;
    }

    NATIVE_FUNCTION(number_imod) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(Number, lhs);
        CONVERT_ARG_TO(0, Number, rhs);

        lhs->set_value((long)lhs->get_value() % (long)rhs->get_value());

        return lhs;
    }

    NATIVE_FUNCTION(number_eq) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(Number, lhs);
        CONVERT_ARG_TO(0, Number, rhs);

        return BOOLEAN(lhs->get_value() == rhs->get_value());
    }

    NATIVE_FUNCTION(number_neq) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(Number, lhs);
        CONVERT_ARG_TO(0, Number, rhs);

        return BOOLEAN(lhs->get_value() != rhs->get_value());
    }

    NATIVE_FUNCTION(number_lt) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(Number, lhs);
        CONVERT_ARG_TO(0, Number, rhs);

        return BOOLEAN(lhs->get_value() < rhs->get_value());
    }

    NATIVE_FUNCTION(number_gt) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(Number, lhs);
        CONVERT_ARG_TO(0, Number, rhs);

        return BOOLEAN(lhs->get_value() > rhs->get_value());
    }

    NATIVE_FUNCTION(number_lte) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(Number, lhs);
        CONVERT_ARG_TO(0, Number, rhs);

        return BOOLEAN(lhs->get_value() <= rhs->get_value());
    }

    NATIVE_FUNCTION(number_gte) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(Number, lhs);
        CONVERT_ARG_TO(0, Number, rhs);

        return BOOLEAN(lhs->get_value() >= rhs->get_value());
    }

    NATIVE_FUNCTION(number_bit_or) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(Number, lhs);
        CONVERT_ARG_TO(0, Number, rhs);

        return ALLOC_NUMBER((long)lhs->get_value() | (long)rhs->get_value());
    }

    NATIVE_FUNCTION(number_bit_xor) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(Number, lhs);
        CONVERT_ARG_TO(0, Number, rhs);

        return ALLOC_NUMBER((long)lhs->get_value() ^ (long)rhs->get_value());
    }

    NATIVE_FUNCTION(number_bit_and) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(Number, lhs);
        CONVERT_ARG_TO(0, Number, rhs);

        return ALLOC_NUMBER((long)lhs->get_value() & (long)rhs->get_value());
    }

    NATIVE_FUNCTION(number_bit_shl) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(Number, lhs);
        CONVERT_ARG_TO(0, Number, rhs);

        return ALLOC_NUMBER((long)lhs->get_value() << (long)rhs->get_value());
    }

    NATIVE_FUNCTION(number_bit_shr) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(Number, lhs);
        CONVERT_ARG_TO(0, Number, rhs);

        return ALLOC_NUMBER((long)lhs->get_value() >> (long)rhs->get_value());
    }

    NATIVE_FUNCTION(number_clone) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(Number, self);

        return context->get_heap().allocate<Number>(context, self);
    }

} // namespace natives
} // namespace emerald
