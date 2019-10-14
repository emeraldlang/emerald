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

#ifndef EMERALD_NATIVES_UTILS_H
#define EMERALD_NATIVES_UTILS_H

#define EXPECT_NUM_ARGS_OP(count, op)                           \
    do {                                                        \
        if (args.size() op count) {                             \
            throw context.get_heap().allocate<Exception>("");   \
        }                                                       \
    } while (false)

#define EXPECT_NUM_ARGS(count) EXPECT_NUM_ARGS_OP(count, !=)
#define EXPECT_ATLEAST_NUM_ARGS(count) EXPECT_NUM_ARGS_OP(count, <)

#define TRY_CONVERT_ARG_TO(i, Type, name)                       \
    Type* name  = nullptr;                                      \
    do {                                                        \
        name = dynamic_cast<Type*>(args[i]);                    \
        if (name == nullptr) {                                  \
            throw context.get_heap().allocate<Exception>("");   \
        }                                                       \
    } while (false)

#define TRY_CONVERT_RECV_TO(Type, name) TRY_CONVERT_ARG_TO(0, Type, name)

#define TRY_CONVERT_OPTIONAL_ARG_TO(i, Type, name)  \
    Type* name;                                     \
    do {                                            \
        if (i < args.size()) {                      \
            name = dynamic_cast<Type*>(args[i]);    \
        } else {                                    \
            name = nullptr;                         \
        }                                           \
    } while (false)


#define BOOLEAN(val) context.get_native_objects().get_boolean(val)
#define NONE Null::get()
#define ALLOC_NUMBER(num) context.get_heap().allocate<Number>(context.get_native_objects().get_number_prototype(), num)
#define ALLOC_STRING(str) context.get_heap().allocate<String>(context.get_native_objects().get_string_prototype(), str)

#endif // EMERALD_NATIVES_UTILS_H
