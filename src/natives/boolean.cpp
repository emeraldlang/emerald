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

#include "fmt/format.h"

#include "emerald/natives/boolean.h"
#include "emerald/natives/utils.h"

namespace emerald {
namespace natives {

#define X(name)                                                             \
    NativeFunction* get_##name() {                                          \
        static NativeFunction func = NativeFunction(std::function(&name));  \
        return &func;                                                       \
    }
    BOOLEAN_NATIVES
#undef X

    NATIVE_FUNCTION(boolean_eq) {
        (void)heap;

        EXPECT_NUM_ARGS(2);

        return BOOLEAN(args[0]->as_bool() == args[1]->as_bool());
    }

    NATIVE_FUNCTION(boolean_neq) {
        (void)heap;

        EXPECT_NUM_ARGS(2);

        return BOOLEAN(args[0]->as_bool() != args[1]->as_bool());
    }

    NATIVE_FUNCTION(boolean_clone) {
        (void)native_objects;

        EXPECT_NUM_ARGS(1);

        TRY_CONVERT_RECV_TO(Boolean, self);

        return heap->allocate<Boolean>(self);
    }

} // namespace natives
} // namepsace emerald
