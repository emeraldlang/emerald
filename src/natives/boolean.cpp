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

#include "emerald/interpreter.h"
#include "emerald/magic_methods.h"
#include "emerald/natives/boolean.h"
#include "emerald/native_frame.h"
#include "emerald/objectutils.h"
#include "emerald/process.h"

namespace emerald {
namespace natives {

    NATIVE_FUNCTION(boolean_eq) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(Boolean, self);
        if (TRY_CONVERT_ARG_TO(0, Boolean, other)) {
            return self->eq(other);
        }

        return BOOLEAN(false);
    }

    NATIVE_FUNCTION(boolean_neq) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(Boolean, self);
        if (TRY_CONVERT_ARG_TO(0, Boolean, other)) {
            return self->neq(other);
        }

        return BOOLEAN(true);
    }

    NATIVE_FUNCTION(boolean_clone) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(Boolean, self);

        return process->get_heap().allocate<Boolean>(process, self);
    }

    NATIVE_FUNCTION(boolean_init) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(Boolean, self);
        Boolean* val = Interpreter::execute_method<Boolean>(
            frame->get_arg(0),
            magic_methods::boolean,
            {},
            process);
        self->init(val);

        return NONE;
    }


} // namespace natives
} // namepsace emerald
