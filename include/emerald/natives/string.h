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

#ifndef _EMERALD_NATIVES_STRING_H
#define _EMERALD_NATIVES_STRING_H

#include "emerald/object.h"

#define STRING_NATIVES      \
    X(string_add)           \
    X(string_eq)            \
    X(string_neq)           \
    X(string_lt)            \
    X(string_gt)            \
    X(string_lte)           \
    X(string_gte)           \
    X(string_clone)         \
    X(string_empty)         \
    X(string_length)        \
    X(string_at)            \
    X(string_back)          \
    X(string_front)         \
    X(string_compare)       \
    X(string_find)          \
    X(string_substr)        \
    X(string_format)        \
    X(string_split)         \
    X(string_append)

namespace emerald {
namespace natives {

#define X(name) NATIVE_FUNCTION(name);
    STRING_NATIVES
#undef X

} // namespace natives
} // namespace emerald

#endif // _EMERALD_NATIVES_STRING_H
