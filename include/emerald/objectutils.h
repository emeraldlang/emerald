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

#ifndef _EMERALD_OBJECTUTILS_H
#define _EMERALD_OBJECTUTILS_H

#include "fmt/format.h"

#include "emerald/execution_context.h"
#include "emerald/interpreter.h"
#include "emerald/magic_methods.h"
#include "emerald/object.h"
#include "emerald/strutils.h"

#define EXPECT_NUM_ARGS_OP(count, op)                           \
    do {                                                        \
        if (args.size() op count) {                             \
            throw context->get_heap().allocate<Exception>(      \
                context, fmt::format(                           \
                    "expected {0} args, got {1}",               \
                    count,                                      \
                    args.size()));                              \
        }                                                       \
    } while (false)

#define EXPECT_NUM_ARGS(count) EXPECT_NUM_ARGS_OP(count, !=)
#define EXPECT_ATLEAST_NUM_ARGS(count) EXPECT_NUM_ARGS_OP(count, <)

#define CONVERT_VAL_TO(val, Type, name)                         \
    Type* name  = nullptr;                                      \
    do {                                                        \
        name = dynamic_cast<Type*>(val);                        \
        if (name == nullptr) {                                  \
            throw context->get_heap().allocate<Exception>(      \
                context, "");                                   \
        }                                                       \
    } while (false)

#define CONVERT_ARG_TO(i, Type, name) CONVERT_VAL_TO(args[i], Type, name)
#define CONVERT_RECV_TO(Type, name) CONVERT_VAL_TO(receiver, Type, name)

#define TRY_CONVERT_VAL_TO(val, Type, name) Type* name = dynamic_cast<Type*>(val)
#define TRY_CONVERT_ARG_TO(i, Type, name) TRY_CONVERT_VAL_TO(args[i], Type, name)
#define TRY_CONVERT_RECV_TO(Type, name) TRY_CONVERT_VAL_TO(receiver, Type, name)

#define TRY_CONVERT_OPTIONAL_ARG_TO(i, Type, name)  \
    Type* name;                                     \
    do {                                            \
        if (i < args.size()) {                      \
            name = dynamic_cast<Type*>(args[i]);    \
        } else {                                    \
            name = nullptr;                         \
        }                                           \
    } while (false)


#define BOOLEAN(val) context->get_native_objects().get_boolean(val)
#define NONE context->get_native_objects().get_null()
#define ALLOC_EMPTY_ARRAY() context->get_heap().allocate<Array>(context)
#define ALLOC_ARRAY(arr) context->get_heap().allocate<Array>(context, arr)
#define ALLOC_NATIVE_FUNCTION(function) context->get_heap().allocate<NativeFunction>(context, function)
#define ALLOC_NUMBER(num) context->get_heap().allocate<Number>(context, num)
#define ALLOC_STRING(str) context->get_heap().allocate<String>(context, str)

namespace emerald {
namespace objectutils {

    template <class InputIt1, class InputIt2>
    inline bool compare_range(InputIt1 first1, InputIt1 last1, InputIt2 first2, ExecutionContext* context) {
        return std::equal(first1, last1, first2, [&context](Object* lhs, Object* rhs) {
            return Interpreter::execute_method(lhs, magic_methods::eq, { rhs }, context);
        });
    }

    template <class InputIt>
    inline std::string join_range(InputIt begin, InputIt end, const std::string& seperator, ExecutionContext* context) {
        return strutils::join(
            begin,
            end,
            seperator,
            [&context](Object* obj) {
                return Interpreter::execute_method(
                    obj,
                    magic_methods::str,
                    {},
                    context)->as_str();
            });
    }

} // namespace objectutils
} // namespace emerald

#endif // _EMERALD_OBJECTUTILS_H
