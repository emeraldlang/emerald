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

#include "emerald/execution_context.h"
#include "emerald/interpreter.h"
#include "emerald/natives/boolean.h"
#include "emerald/magic_methods.h"
#include "emerald/objectutils.h"

namespace emerald {
namespace natives {

    NATIVE_FUNCTION(boolean_eq) {
        EXPECT_NUM_ARGS(2);

        CONVERT_RECV_TO(Boolean, self);
        Object* other = Interpreter::execute_method(magic_methods::boolean, { args[1] }, context);

        return BOOLEAN(self->as_bool() == other->as_bool());
    }

    NATIVE_FUNCTION(boolean_neq) {
        return BOOLEAN(!boolean_eq(args, context)->as_bool());
    }

    NATIVE_FUNCTION(boolean_clone) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(Boolean, self);

        return context->get_heap().allocate<Boolean>(context, self);
    }

} // namespace natives
} // namepsace emerald
