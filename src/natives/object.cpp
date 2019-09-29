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
        EXPECT_NUM_ARGS(2);

        return heap->allocate<Boolean>(
            native_prototypes->get_boolean_prototype(),
            args[0] == args[1]);
    }

    NATIVE_FUNCTION(object_neq) {
        EXPECT_NUM_ARGS(2);

        return heap->allocate<Boolean>(
            native_prototypes->get_boolean_prototype(),
            args[0] != args[1]);
    }

    NATIVE_FUNCTION(object_clone) {
        EXPECT_NUM_ARGS(0);

        return heap->allocate<HeapObject>(
            native_prototypes->get_object_prototype());
    }

} // namespace natives
} // namespace emerald
