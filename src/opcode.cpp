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

#include "emerald/opcode.h"

namespace emerald {

    const std::string OpCode::get_string(Value op) {
        return _strings[op];
    }

    uint8_t OpCode::get_arg_count(Value op) {
        return _arg_counts[op];
    }

#define X(name, arg_count) #name,
    const std::string OpCode::_strings[] = { _OPCODES };
#undef X

#define X(name, arg_count) arg_count,
    const uint8_t OpCode::_arg_counts[] = { _OPCODES };
#undef X 

} // namespace emerald
