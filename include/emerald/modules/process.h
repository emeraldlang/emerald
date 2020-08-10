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

#ifndef _EMERALD_MODULES_PROCESS_H
#define _EMERALD_MODULES_PROCESS_H

#include "emerald/module_registry.h"
#include "emerald/object.h"

#define PROCESS_NATIVES \
    X(process_create)   \
    X(process_id)       \
    X(process_join)     \
    X(process_receive)  \
    X(process_send)     \
    X(process_sleep)    \
    X(process_state)

namespace emerald {
namespace modules {

#define X(name) NATIVE_FUNCTION(name);
    PROCESS_NATIVES
#undef X

    MODULE_INITIALIZATION_FUNC(init_process_module);

} // namespace modules
} // namespace emerald

#endif // _EMERALD_MODULES_PROCESS_H
