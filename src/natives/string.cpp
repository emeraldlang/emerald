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

#include "fmt/format.h"

#include "emerald/execution_context.h"
#include "emerald/natives/string.h"
#include "emerald/native_frame.h"
#include "emerald/objectutils.h"
#include "emerald/strutils.h"

namespace emerald {
namespace natives {

    NATIVE_FUNCTION(string_add) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(String, self);
        CONVERT_ARG_TO(0, String, other);

        return ALLOC_STRING(self->get_value() + other->get_value());
    }

    NATIVE_FUNCTION(string_eq) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(String, self);
        CONVERT_ARG_TO(0, String, other);

        return BOOLEAN(self->get_value() == other->get_value());
    }

    NATIVE_FUNCTION(string_neq) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(String, self);
        CONVERT_ARG_TO(0, String, other);

        return BOOLEAN(self->get_value() != other->get_value());
    }

    NATIVE_FUNCTION(string_lt) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(String, self);
        CONVERT_ARG_TO(0, String, other);

        return BOOLEAN(self->get_value() < other->get_value());
    }

    NATIVE_FUNCTION(string_gt) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(String, self);
        CONVERT_ARG_TO(0, String, other);

        return BOOLEAN(self->get_value() > other->get_value());
    }

    NATIVE_FUNCTION(string_lte) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(String, self);
        CONVERT_ARG_TO(0, String, other);

        return BOOLEAN(self->get_value() <= other->get_value());
    }

    NATIVE_FUNCTION(string_gte) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(String, self);
        CONVERT_ARG_TO(0, String, other);

        return BOOLEAN(self->get_value() > other->get_value());
    }

    NATIVE_FUNCTION(string_clone) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(String, self);

        return context->get_heap().allocate<String>(context, self);
    }

    NATIVE_FUNCTION(string_empty) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(String, self);

        return BOOLEAN(self->get_value().empty());
    }

    NATIVE_FUNCTION(string_length) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(String, self);

        return ALLOC_NUMBER(self->get_value().size());
    }

    NATIVE_FUNCTION(string_at) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(String, self);
        CONVERT_ARG_TO(0, Number, index);

        return ALLOC_STRING(std::string(1, self->get_value()[(long)index->get_value()]));
    }

    NATIVE_FUNCTION(string_back) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(String, self);

        return ALLOC_STRING(std::string(1, self->get_value().back()));
    }

    NATIVE_FUNCTION(string_front) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(String, self);

        return ALLOC_STRING(std::string(1, self->get_value().back()));
    }

    NATIVE_FUNCTION(string_compare) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(String, self);
        CONVERT_ARG_TO(0, String, other);

        return ALLOC_NUMBER(self->get_value().compare(other->get_value()));
    }

    NATIVE_FUNCTION(string_find) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(String, self);
        CONVERT_ARG_TO(0, String, other);

        return ALLOC_NUMBER(self->get_value().find(other->get_value()));
    }

    NATIVE_FUNCTION(string_substr) {
        EXPECT_NUM_ARGS(2);

        CONVERT_RECV_TO(String, self);
        CONVERT_ARG_TO(0, Number, pos);
        CONVERT_ARG_TO(1, Number, len);

        return ALLOC_STRING(self->get_value().substr(pos->get_value(), len->get_value()));
    }

    NATIVE_FUNCTION(string_format) {
        CONVERT_RECV_TO(String, self);

        using ctx = fmt::format_context;
        std::vector<fmt::basic_format_arg<ctx>> fmt_args;
        size_t n = frame->num_args();
        for (size_t i = 0; i < n; i++) {
            Object* arg = Interpreter::execute_method(frame->get_arg(i), magic_methods::str, {}, context);
            fmt_args.push_back(fmt::internal::make_arg<ctx>(arg->as_str()));
        }

        return ALLOC_STRING(
            fmt::vformat(self->get_value(), fmt::basic_format_args<ctx>(fmt_args.data(), fmt_args.size())));
    }

    NATIVE_FUNCTION(string_split) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(String, self);
        CONVERT_ARG_TO(0, String, seperator);

        LOCAL(Array, res, ALLOC_EMPTY_ARRAY());
        for (const std::string& part : strutils::split(
                self->get_value(),
                seperator->get_value())) {
            res->push(ALLOC_STRING(part));
        }

        return res;
    }

    NATIVE_FUNCTION(string_append) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(String, self);
        CONVERT_ARG_TO(0, String, str);

        self->get_value().append(str->get_value());

        return self;
    }

} // namespace natives
} // namespace emerald
