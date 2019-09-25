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
        Module* module = heap->allocate<Module>("core");

        module->set_property("Object", native_prototypes->get_object_prototype());
        module->set_property("Array", native_prototypes->get_array_prototype());
        module->set_property("Boolean", native_prototypes->get_boolean_prototype());
        module->set_property("Number", native_prototypes->get_number_prototype());
        module->set_property("String", native_prototypes->get_string_prototype());

        return module;
    }

} // namespace modules
} // namespace emerald
