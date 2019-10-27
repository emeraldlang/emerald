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

#define _MAGIC_METHODS          \
    /* Unary Operators */       \
    X(neg)                      \
    /* Arithmetic */            \
    X(add)                      \
    X(sub)                      \
    X(mul)                      \
    X(div)                      \
    X(mod)                      \
    /* Augmented Assignment */  \
    X(iadd)                     \
    X(isub)                     \
    X(imul)                     \
    X(idiv)                     \
    X(imod)                     \
    /* Comparison */            \
    X(eq)                       \
    X(neq)                      \
    X(lt)                       \
    X(gt)                       \
    X(lte)                      \
    X(gte)                      \
    /* Bitwise */               \
    X(bit_not)                  \
    X(bit_or)                   \
    X(bit_xor)                  \
    X(bit_and)                  \
    X(bit_shl)                  \
    X(bit_shr)                  \
    /* Casts */                 \
    X(str)                      \
    X(boolean)                  \
    /* Iterators */             \
    X(iter)                     \
    X(cur)                      \
    X(done)                     \
    X(next)                     \
    /* Other */                 \
    X(call)                     \
    X(clone)                    \
    X(init)

#define X(name) const std::string name = "__" #name "__";
_MAGIC_METHODS
#undef X

} // namespace magic_methods
} // namespace emerald

#endif // _EMERALD_MAGIC_METHODS_H
