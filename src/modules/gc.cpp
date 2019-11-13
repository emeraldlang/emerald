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

#include "emerald/module.h"
#include "emerald/modules/gc.h"
#include "emerald/native_frame.h"
#include "emerald/objectutils.h"
#include "emerald/process.h"

namespace emerald {
namespace modules {

    NATIVE_FUNCTION(gc_collect) {
        process->get_heap().collect();
        return NONE;
    }

    NATIVE_FUNCTION(gc_total_allocated_objects) {
        return ALLOC_NUMBER(process->get_heap().get_managed_count());
    }

    NATIVE_FUNCTION(gc_threshold) {
        return ALLOC_NUMBER(process->get_heap().threshold());
    }

    NATIVE_FUNCTION(gc_set_threshold) {
        EXPECT_NUM_ARGS(1);

        CONVERT_ARG_TO(0, Number, threshold);
        process->get_heap().set_threshold(threshold->get_native_value());

        return NONE;
    }

    MODULE_INITIALIZATION_FUNC(init_gc_module) {
        Module* module = ALLOC_MODULE("gc");

        module->set_property("collect", ALLOC_NATIVE_FUNCTION(gc_collect));
        module->set_property("total_allocated_objects", ALLOC_NATIVE_FUNCTION(gc_total_allocated_objects));
        module->set_property("threshold", ALLOC_NATIVE_FUNCTION(gc_threshold));
        module->set_property("set_threshold", ALLOC_NATIVE_FUNCTION(gc_set_threshold));

        return module;
    }

} // namespace modules
} // namespace emerald
