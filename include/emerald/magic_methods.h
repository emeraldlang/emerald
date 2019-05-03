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

#ifndef _EMERALD_MAGIC_METHODS_H
#define _EMERALD_MAGIC_METHODS_H

namespace emerald {
namespace magic_methods {

#define _MAGIC_METHODS  \
    /* Arithmetic */    \
    X(add)              \
    X(sub)              \
    X(mul)              \
    X(div)              \
    X(mod)              \
    X(prefix_inc)       \
    X(prefix_dec)       \
    X(postfix_inc)      \
    X(postfix_dec)      \
    /* Comparison */    \
    X(eq)               \
    X(neq)              \
    X(lt)               \
    X(gt)               \
    X(lte)              \
    X(gte)              \
    /* Bitwise */       \
    X(bit_or)           \
    X(bit_xor)          \
    X(bit_and)          \
    /* Casts */         \
    X(str)              \
    X(boolean)          \
    /* Other */         \
    X(call)             \
    X(clone)

#define X(name) const std::string name = "__" #name "__";
_MAGIC_METHODS
#undef X

} // namespace magic_methods
} // namespace emerald

#endif // _EMERALD_MAGIC_METHODS_H
