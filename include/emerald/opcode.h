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

#ifndef _EMERALD_OPCODE_H
#define _EMERALD_OPCODE_H

#include <cstdint>
#include <string>

namespace emerald {

// x macro (name, arg_count)
#define _OPCODES            \
    /* Control */           \
    X(nop, 0)               \
    X(jmp, 1)               \
    X(jmp_true, 1)          \
    X(jmp_false, 1)         \
    /* Arithmetic */        \
    X(add, 0)               \
    X(sub, 0)               \
    X(mul, 0)               \
    X(div, 0)               \
    X(mod, 0)               \
    X(prefix_inc, 0)        \
    X(prefix_dec, 0)        \
    X(postfix_inc, 0)       \
    X(postfix_dec, 0)       \
    /* Comparison */        \
    X(eq, 0)                \
    X(neq, 0)               \
    X(lt, 0)                \
    X(gt, 0)                \
    X(lte, 0)               \
    X(gte, 0)               \
    /* Bitwise */           \
    X(bit_or, 0)            \
    X(bit_xor, 0)           \
    X(bit_and, 0)           \
    /* Cast Operators */    \
    X(str, 0)               \
    X(boolean, 0)           \
    /* Functions */         \
    X(call, 1)              \
    X(ret, 0)               \
    /* Objects */           \
    X(newobj, 2)            \
    X(newfunc, 1)           \
    X(newnum, 1)            \
    X(newstr, 1)            \
    X(getprop, 0)           \
    X(hasprop, 0)           \
    X(setprop, 0)           \
    /* Loads & Stores */    \
    X(ldloc, 1)             \
    X(stloc, 1)             \
    /* Other */             \
    X(print, 0)             \
    X(allocate_locals, 1)

    class OpCode {
    public:
#define X(name, arg_count) name,
        enum Value {
            _OPCODES
            NUM_OPCODES
        };
#undef X

        static const std::string get_string(Value op);
        static uint8_t get_arg_count(Value op);

    private:
        static const std::string _strings[NUM_OPCODES];
        static const uint8_t _arg_counts[NUM_OPCODES];
    };

} // namespace emerald

#endif // _EMERALD_OPCODE_H
