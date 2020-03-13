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
#include "emerald/module_registry.h"
#include "emerald/modules/bytecode.h"
#include "emerald/modules/collections.h"
#include "emerald/modules/core.h"
#include "emerald/modules/datetime.h"
#include "emerald/modules/gc.h"
#include "emerald/modules/io.h"
#include "emerald/modules/net.h"
#include "emerald/modules/process.h"

namespace emerald {
namespace modules {

    void add_module_inits_to_registry() {
        NativeModuleInitRegistry::add_module_init("bytecode", init_bytecode_module);
        NativeModuleInitRegistry::add_module_init("collections", init_collections_module);
        NativeModuleInitRegistry::add_module_init("core", init_core_module);
        NativeModuleInitRegistry::add_module_init("datetime", init_datetime_module);
        NativeModuleInitRegistry::add_module_init("gc", init_gc_module);
        NativeModuleInitRegistry::add_module_init("io", init_io_module);
        NativeModuleInitRegistry::add_module_init("net", init_net_module);
        NativeModuleInitRegistry::add_module_init("process", init_process_module);
    }

} // namespace modules
} // namespace emerald
