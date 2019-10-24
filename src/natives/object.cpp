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

#include "emerald/execution_context.h"
#include "emerald/natives/object.h"
#include "emerald/objectutils.h"

namespace emerald {
namespace natives {

    NATIVE_FUNCTION(object_eq) {
        EXPECT_NUM_ARGS(2);

        return BOOLEAN(args[0] == args[1]);
    }

    NATIVE_FUNCTION(object_neq) {
        EXPECT_NUM_ARGS(2);

        return BOOLEAN(args[0] != args[1]);
    }

    NATIVE_FUNCTION(object_str) {
        EXPECT_NUM_ARGS(1);

        return ALLOC_STRING(args[0]->as_str());
    }

    NATIVE_FUNCTION(object_boolean) {
        EXPECT_NUM_ARGS(1);

        return BOOLEAN(args[0]->as_bool());
    }

    NATIVE_FUNCTION(object_clone) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(Object, self);

        return context->get_heap().allocate<Object>(context, self);
    }

    NATIVE_FUNCTION(object_init) {
        return NONE;
    }

    NATIVE_FUNCTION(object_keys) {
        EXPECT_NUM_ARGS(1);

        Array* keys = ALLOC_EMPTY_ARRAY();
        for (const auto& pair : args[0]->get_properties()) {
            keys->push(ALLOC_STRING(pair.first));
        }

        return keys;
    }

    NATIVE_FUNCTION(object_get_prop) {
        EXPECT_NUM_ARGS(3);

        TRY_CONVERT_ARG_TO(1, String, name);

        if (Object* property = args[0]->get_property(name->as_str())) {
            return property;
        }

        return args[2];
    }

    NATIVE_FUNCTION(object_set_prop) {
        EXPECT_NUM_ARGS(3);

        TRY_CONVERT_ARG_TO(1, String, name);
        args[0]->set_property(name->as_str(), args[2]);

        return NONE;
    }

} // namespace natives
} // namespace emerald
