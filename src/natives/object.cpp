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
#include "emerald/natives/utils.h"

namespace emerald {
namespace natives {

#define X(name)                                                             \
    NativeFunction* get_##name() {                                          \
        static NativeFunction func = NativeFunction(std::function(&name));  \
        return &func;                                                       \
    }
    OBJECT_NATIVES
#undef X

    NATIVE_FUNCTION(object_eq) {
        (void)heap;

        EXPECT_NUM_ARGS(2);

        return BOOLEAN(args[0] == args[1]);
    }

    NATIVE_FUNCTION(object_neq) {
        (void)heap;

        EXPECT_NUM_ARGS(2);

        return BOOLEAN(args[0] != args[1]);
    }

    NATIVE_FUNCTION(object_str) {
        EXPECT_NUM_ARGS(1);

        return ALLOC_STRING(args[0]->as_str());
    }

    NATIVE_FUNCTION(object_boolean) {
        (void)heap;

        EXPECT_NUM_ARGS(1);

        return BOOLEAN(args[0]->as_bool());
    }

    NATIVE_FUNCTION(object_clone) {
        (void)native_objects;

        EXPECT_NUM_ARGS(1);

        TRY_CONVERT_RECV_TO(HeapObject, self);

        return heap->allocate<HeapObject>(self);
    }

    NATIVE_FUNCTION(object_init) {
        (void)native_objects;
        (void)heap;

        EXPECT_NUM_ARGS(1);

        TRY_CONVERT_RECV_TO(HeapObject, self);

        return self;
    }

} // namespace natives
} // namespace emerald
