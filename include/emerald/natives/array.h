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

#ifndef _EMERALD_NATIVES_ARRAY_H
#define _EMERALD_NATIVES_ARRAY_H

#include "emerald/object.h"

#define ARRAY_NATIVES   \
    X(array_eq)         \
    X(array_neq)        \
    X(array_iter)       \
    X(array_clone)      \
    X(array_at)         \
    X(array_front)      \
    X(array_back)       \
    X(array_empty)      \
    X(array_size)       \
    X(array_clear)      \
    X(array_push)       \
    X(array_pop)        \
    X(array_join)

#define ARRAY_ITERATOR_NATIVES  \
    X(array_iterator_cur)       \
    X(array_iterator_done)      \
    X(array_iterator_next)      \
    X(array_iterator_clone)     \
    X(array_iterator_init)

namespace emerald {
namespace natives {

#define X(name) NATIVE_FUNCTION(name);
    ARRAY_NATIVES
    ARRAY_ITERATOR_NATIVES
#undef X

} // namespace natives
} // namespace emerald

#endif // _EMERALD_NATIVES_ARRAY_H
