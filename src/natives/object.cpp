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

#include "emerald/natives/object.h"
#include "emerald/native_variables.h"
#include "emerald/objectutils.h"
#include "emerald/process.h"

namespace emerald {
namespace natives {

    NATIVE_FUNCTION(object_eq) {
        EXPECT_NUM_ARGS(1);

        return BOOLEAN(frame->get_receiver() == frame->get_arg(0));
    }

    NATIVE_FUNCTION(object_neq) {
        EXPECT_NUM_ARGS(1);

        return BOOLEAN(frame->get_receiver() != frame->get_arg(0));
    }

    NATIVE_FUNCTION(object_str) {
        EXPECT_NUM_ARGS(0);

        return ALLOC_STRING(frame->get_receiver()->as_str());
    }

    NATIVE_FUNCTION(object_boolean) {
        EXPECT_NUM_ARGS(0);

        return BOOLEAN(frame->get_receiver()->as_bool());
    }

    NATIVE_FUNCTION(object_clone) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(Object, self);

        return process->get_heap().allocate<Object>(process, self);
    }

    NATIVE_FUNCTION(object_init) {
        return NONE;
    }

    NATIVE_FUNCTION(object_keys) {
        EXPECT_NUM_ARGS(0);

        Local<Array> keys = ALLOC_EMPTY_ARRAY();
        for (const auto& pair : frame->get_receiver()->get_properties()) {
            keys->push(ALLOC_STRING(pair.first));
        }

        return keys.val();
    }

    NATIVE_FUNCTION(object_get_prop) {
        EXPECT_NUM_ARGS(2);

        CONVERT_ARG_TO(0, String, name);

        if (Object* property = frame->get_receiver()->get_property(name->get_native_value())) {
            return property;
        }

        return frame->get_arg(1);
    }

    NATIVE_FUNCTION(object_set_prop) {
        EXPECT_NUM_ARGS(2);

        CONVERT_ARG_TO(0, String, name);
        frame->get_receiver()->set_property(name->get_native_value(), frame->get_arg(1));

        return NONE;
    }

} // namespace natives
} // namespace emerald
