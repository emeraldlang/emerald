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

#ifndef _EMERALD_NATIVES_OBJECT_H
#define _EMERALD_NATIVES_OBJECT_H

#include "emerald/object.h"
#include "emerald/native_objects.h"

#define OBJECT_NATIVES  \
    X(object_eq)        \
    X(object_neq)       \
    X(object_str)       \
    X(object_boolean)   \
    X(object_clone)     \
    X(object_init)

namespace emerald {
namespace natives {

#define X(name) NativeFunction* get_##name();
    OBJECT_NATIVES
#undef X

#define X(name) NATIVE_FUNCTION(name);
    OBJECT_NATIVES
#undef X

} // namespace natives
} // namespace emerald
#endif // _EMERALD_NATIVES_OBJECT_H
