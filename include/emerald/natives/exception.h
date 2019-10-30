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

#ifndef _EMERALD_NATIVES_EXCEPTION_H
#define _EMERALD_NATIVES_EXCEPTION_H

#include "emerald/object.h"

#define EXCEPTION_NATIVES   \
    X(exception_clone)      \
    X(exception_init)

namespace emerald {
namespace natives {

#define X(name) NATIVE_FUNCTION(name);
    EXCEPTION_NATIVES
#undef X

} // namespace natives
} // namespace emerald

#endif // _EMERALD_NATIVES_EXCEPTION_H
