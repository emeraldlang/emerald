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

#include "emerald/modules/core.h"

namespace emerald {
namespace modules {

    MODULE_INITIALIZATION_FUNC(init_core_module) {
        (void)heap;

        std::shared_ptr<Module> module = std::make_shared<Module>("io");

        module->add_object("Object", native_prototypes->get_object_prototype());
        module->add_object("Array", native_prototypes->get_array_prototype());
        module->add_object("Boolean", native_prototypes->get_boolean_prototype());
        module->add_object("Number", native_prototypes->get_number_prototype());
        module->add_object("String", native_prototypes->get_string_prototype());

        return module;
    }

} // namespace modules
} // namespace emerald
