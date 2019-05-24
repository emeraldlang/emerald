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

#include "emerald/modules/init.h"
#include "emerald/module.h"
#include "emerald/modules/core.h"
#include "emerald/modules/io.h"

namespace emerald {
namespace modules {

    void add_modules_to_registry() {
        NativeModuleRegistry::add_module("core", init_core_module);
        NativeModuleRegistry::add_module("io", init_io_module);
    }

} // namespace modules
} // namespace emerald