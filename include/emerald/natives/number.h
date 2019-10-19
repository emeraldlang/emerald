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

#ifndef _EMERALD_NATIVES_NUMBER_H
#define _EMERALD_NATIVES_NUMBER_H

#include "emerald/object.h"

#define NUMBER_NATIVES  \
    X(number_neg)       \
    X(number_add)       \
    X(number_sub)       \
    X(number_mul)       \
    X(number_div)       \
    X(number_mod)       \
    X(number_iadd)      \
    X(number_isub)      \
    X(number_imul)      \
    X(number_idiv)      \
    X(number_imod)      \
    X(number_eq)        \
    X(number_neq)       \
    X(number_lt)        \
    X(number_gt)        \
    X(number_lte)       \
    X(number_gte)       \
    X(number_bit_or)    \
    X(number_bit_xor)   \
    X(number_bit_and)   \
    X(number_bit_shl)   \
    X(number_bit_shr)   \
    X(number_clone)

namespace emerald {
namespace natives {

#define X(name) NATIVE_FUNCTION(name);
    NUMBER_NATIVES
#undef X

} // namespace natives
} // namespace emerald

#endif // _EMERALD_NATIVES_NUMBER_H
