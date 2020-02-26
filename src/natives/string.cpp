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

#include "emerald/natives/string.h"
#include "emerald/native_variables.h"
#include "emerald/objectutils.h"
#include "emerald/process.h"
#include "emerald/strutils.h"

namespace emerald {
namespace natives {

    NATIVE_FUNCTION(string_add) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(String, self);
        CONVERT_ARG_TO(0, String, other);

        return ALLOC_STRING(self->get_native_value() + other->get_native_value());
    }

    NATIVE_FUNCTION(string_eq) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(String, self);
        CONVERT_ARG_TO(0, String, other);

        return BOOLEAN(self->get_native_value() == other->get_native_value());
    }

    NATIVE_FUNCTION(string_neq) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(String, self);
        CONVERT_ARG_TO(0, String, other);

        return BOOLEAN(self->get_native_value() != other->get_native_value());
    }

    NATIVE_FUNCTION(string_lt) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(String, self);
        CONVERT_ARG_TO(0, String, other);

        return BOOLEAN(self->get_native_value() < other->get_native_value());
    }

    NATIVE_FUNCTION(string_gt) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(String, self);
        CONVERT_ARG_TO(0, String, other);

        return BOOLEAN(self->get_native_value() > other->get_native_value());
    }

    NATIVE_FUNCTION(string_lte) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(String, self);
        CONVERT_ARG_TO(0, String, other);

        return BOOLEAN(self->get_native_value() <= other->get_native_value());
    }

    NATIVE_FUNCTION(string_gte) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(String, self);
        CONVERT_ARG_TO(0, String, other);

        return BOOLEAN(self->get_native_value() > other->get_native_value());
    }

    NATIVE_FUNCTION(string_iadd) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(String, self);
        CONVERT_ARG_TO(0, String, str);

        self->get_native_value().append(str->get_native_value());

        return self;
    }

    NATIVE_FUNCTION(string_clone) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(String, self);

        return process->get_heap().allocate<String>(process, self);
    }

    NATIVE_FUNCTION(string_empty) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(String, self);

        return BOOLEAN(self->get_native_value().empty());
    }

    NATIVE_FUNCTION(string_length) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(String, self);

        return ALLOC_NUMBER(self->get_native_value().size());
    }

    NATIVE_FUNCTION(string_at) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(String, self);
        CONVERT_ARG_TO(0, Number, index);

        return ALLOC_STRING(std::string(1, self->get_native_value()[(long)index->get_native_value()]));
    }

    NATIVE_FUNCTION(string_back) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(String, self);

        return ALLOC_STRING(std::string(1, self->get_native_value().back()));
    }

    NATIVE_FUNCTION(string_front) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(String, self);

        return ALLOC_STRING(std::string(1, self->get_native_value().back()));
    }

    NATIVE_FUNCTION(string_compare) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(String, self);
        CONVERT_ARG_TO(0, String, other);

        return ALLOC_NUMBER(self->get_native_value().compare(other->get_native_value()));
    }

    NATIVE_FUNCTION(string_find) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(String, self);
        CONVERT_ARG_TO(0, String, other);

        return ALLOC_NUMBER(self->get_native_value().find(other->get_native_value()));
    }

    NATIVE_FUNCTION(string_substr) {
        EXPECT_NUM_ARGS(2);

        CONVERT_RECV_TO(String, self);
        CONVERT_ARG_TO(0, Number, pos);
        CONVERT_ARG_TO(1, Number, len);

        return ALLOC_STRING(self->get_native_value().substr(pos->get_native_value(), len->get_native_value()));
    }

    NATIVE_FUNCTION(string_format) {
        CONVERT_RECV_TO(String, self);

        std::vector<fmt::format_args::format_arg> fmt_args;
        size_t n = frame->num_args();
        for (size_t i = 0; i < n; i++) {
            String* arg = Interpreter::execute_method<String>(frame->get_arg(i), magic_methods::str, {}, process);
            fmt_args.push_back(fmt::internal::make_arg<fmt::format_context>(arg->get_native_value()));
        }

        return ALLOC_STRING(
            fmt::vformat(self->get_native_value(), fmt::format_args(fmt_args.data(), fmt_args.size())));
    }

    NATIVE_FUNCTION(string_split) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(String, self);
        CONVERT_ARG_TO(0, String, seperator);

        Local<Array> res = ALLOC_EMPTY_ARRAY();
        for (const std::string& part : strutils::split(
                self->get_native_value(),
                seperator->get_native_value())) {
            res->push(ALLOC_STRING(part));
        }

        return res.val();
    }

    NATIVE_FUNCTION(string_append) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(String, self);
        CONVERT_ARG_TO(0, String, str);

        self->get_native_value().append(str->get_native_value());

        return self;
    }

    NATIVE_FUNCTION(string_isalpha) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(String, self);

        for (char ch : self->get_native_value()) {
            if (!std::isalpha(ch)) return FALSE;
        }

        return TRUE;
    }

    NATIVE_FUNCTION(string_isalnum) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(String, self);

        for (char ch : self->get_native_value()) {
            if (!std::isalnum(ch)) return FALSE;
        }

        return TRUE;
    }

    NATIVE_FUNCTION(string_isdigit) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(String, self);

        for (char ch : self->get_native_value()) {
            if (!std::isdigit(ch)) return FALSE;
        }

        return TRUE;
    }

} // namespace natives
} // namespace emerald
