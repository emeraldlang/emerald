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
#include "emerald/natives/utils.h"

namespace emerald {
namespace natives {

#define X(name)                                                             \
    NativeFunction* get_##name() {                                          \
        static NativeFunction func = NativeFunction(std::function(&name));  \
        return &func;                                                       \
    }
    STRING_NATIVES
#undef X

    NATIVE_FUNCTION(string_add) {
        EXPECT_NUM_ARGS(2);

        return ALLOC_STRING(args[0]->as_str() + args[1]->as_str());
    }

    NATIVE_FUNCTION(string_eq) {
        EXPECT_NUM_ARGS(2);

        return BOOLEAN(args[0]->as_str() == args[1]->as_str());
    }

    NATIVE_FUNCTION(string_neq) {
        EXPECT_NUM_ARGS(2);

        return BOOLEAN(args[0]->as_str() != args[1]->as_str());
    }

    NATIVE_FUNCTION(string_lt) {
        EXPECT_NUM_ARGS(2);

        return BOOLEAN(args[0]->as_str() < args[1]->as_str());
    }

    NATIVE_FUNCTION(string_gt) {
        EXPECT_NUM_ARGS(2);

        return BOOLEAN(args[0]->as_str() > args[1]->as_str());
    }

    NATIVE_FUNCTION(string_lte) {
        EXPECT_NUM_ARGS(2);

        return BOOLEAN(args[0]->as_str() <= args[1]->as_str());
    }

    NATIVE_FUNCTION(string_gte) {
        EXPECT_NUM_ARGS(2);

        return BOOLEAN(args[0]->as_str() > args[1]->as_str());
    }

    NATIVE_FUNCTION(string_clone) {
        EXPECT_NUM_ARGS(1);

        TRY_CONVERT_RECV_TO(String, self);

        return context.get_heap().allocate<String>(self);
    }

    NATIVE_FUNCTION(string_empty) {
        EXPECT_NUM_ARGS(1);

        return BOOLEAN(args[0]->as_str().empty());
    }

    NATIVE_FUNCTION(string_length) {
        EXPECT_NUM_ARGS(1);

        return ALLOC_NUMBER(args[0]->as_str().size());
    }

    NATIVE_FUNCTION(string_at) {
        EXPECT_NUM_ARGS(2);

        TRY_CONVERT_ARG_TO(1, Number, index);

        return ALLOC_STRING(std::string(1, args[0]->as_str()[(long)index->get_value()]));
    }

    NATIVE_FUNCTION(string_back) {
        EXPECT_NUM_ARGS(1);

        return ALLOC_STRING(std::string(1, args[0]->as_str().back()));
    }

    NATIVE_FUNCTION(string_front) {
        EXPECT_NUM_ARGS(1);

        return ALLOC_STRING(std::string(1, args[0]->as_str().back()));
    }

    NATIVE_FUNCTION(string_compare) {
        EXPECT_NUM_ARGS(2);

        return ALLOC_NUMBER(args[0]->as_str().compare(args[1]->as_str()));
    }

    NATIVE_FUNCTION(string_find) {
        EXPECT_NUM_ARGS(2);

        return ALLOC_NUMBER(args[0]->as_str().find(args[1]->as_str()));
    }

    NATIVE_FUNCTION(string_substr) {
        EXPECT_NUM_ARGS(3);

        TRY_CONVERT_ARG_TO(1, Number, pos);
        TRY_CONVERT_ARG_TO(2, Number, len);

        return ALLOC_STRING(args[0]->as_str().substr(pos->get_value(), len->get_value()));
    }

    NATIVE_FUNCTION(string_format) {
        EXPECT_ATLEAST_NUM_ARGS(1);

        TRY_CONVERT_RECV_TO(String, self);

        using ctx = fmt::format_context;
        std::vector<fmt::basic_format_arg<ctx>> fmt_args;
        for (size_t i = 1; i < args.size(); i++) {
            fmt_args.push_back(fmt::internal::make_arg<ctx>(args[i]->as_str()));
        }

        return ALLOC_STRING(
            fmt::vformat(self->get_value(), fmt::basic_format_args<ctx>(fmt_args.data(), fmt_args.size())));
    }

} // namespace natives
} // namespace emerald
