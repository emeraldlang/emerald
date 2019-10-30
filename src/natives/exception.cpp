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
#include "emerald/natives/exception.h"
#include "emerald/native_frame.h"
#include "emerald/objectutils.h"

namespace emerald {
namespace natives {

    NATIVE_FUNCTION(exception_clone) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(Exception, self);

        return context->get_heap().allocate<Exception>(context, self);
    }

    NATIVE_FUNCTION(exception_init) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(Exception, self);
        CONVERT_ARG_TO(0, String, msg);
        self->init(msg);

        return NONE;
    }

} // namespace natives
} // namespace emerald
