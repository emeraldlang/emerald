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
#define _OPCODES                \
    /* Control */               \
    X(nop, 0)                   \
    X(jmp, 1)                   \
    X(jmp_true, 1)              \
    X(jmp_true_or_pop, 1)       \
    X(jmp_false, 1)             \
    X(jmp_false_or_pop, 1)      \
    X(jmp_data, 1)              \
    /* Stack Manipulation */    \
    X(pop, 1)                   \
    /* Unary */                 \
    X(neg, 0)                   \
    X(log_neg, 0)               \
    /* Arithmetic */            \
    X(add, 0)                   \
    X(sub, 0)                   \
    X(mul, 0)                   \
    X(div, 0)                   \
    X(mod, 0)                   \
    /* Augmented Assignment */  \
    X(iadd, 0)                  \
    X(isub, 0)                  \
    X(imul, 0)                  \
    X(idiv, 0)                  \
    X(imod, 0)                  \
    /* Comparison */            \
    X(eq, 0)                    \
    X(neq, 0)                   \
    X(lt, 0)                    \
    X(gt, 0)                    \
    X(lte, 0)                   \
    X(gte, 0)                   \
    /* Bitwise */               \
    X(bit_not, 0)               \
    X(bit_or, 0)                \
    X(bit_xor, 0)               \
    X(bit_and, 0)               \
    X(bit_shl, 0)               \
    X(bit_shr, 0)               \
    /* Cast Operators */        \
    X(str, 0)                   \
    X(boolean, 0)               \
    /* Functions */             \
    X(call, 2)                  \
    X(ret, 0)                   \
    /* Objects */               \
    X(new_obj, 2)               \
    X(init, 1)                  \
    X(new_func, 1)              \
    X(new_num, 1)               \
    X(new_str, 1)               \
    X(new_boolean, 1)           \
    X(new_arr, 1)               \
    X(null, 0)                  \
    X(get_prop, 1)              \
    X(set_prop, 1)              \
    X(self, 0)                  \
    /* Exceptions */            \
    X(enter_try, 1)             \
    X(exit_try, 1)              \
    X(throw_exc, 0)             \
    /* Iterators */             \
    X(get_iter, 0)              \
    X(iter_cur, 0)              \
    X(iter_done, 0)             \
    X(iter_next, 0)             \
    /* Loads & Stores */        \
    X(ldgbl, 1)                 \
    X(stgbl, 1)                 \
    X(ldloc, 1)                 \
    X(stloc, 1)                 \
    /* Other */                 \
    X(import, 1)

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
