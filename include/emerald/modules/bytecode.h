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

#ifndef _EMERALD_MODULES_BYTECODE_H
#define _EMERALD_MODULES_BYTECODE_H

#include "emerald/module_registry.h"
#include "emerald/object.h"

#define BYTECODE_NATIVES    \
    X(bytecode_bytecode)

#define BYTECODE_ITERATOR_NATIVES   \
    X(bytecode_iterator_clone)      \
    X(bytecode_iterator_init)       \
    X(bytecode_iterator_cur)        \
    X(bytecode_iterator_done)       \
    X(bytecode_iterator_next)

namespace emerald {
namespace modules {

    class BytecodeIterator : public Object {
    public:
        BytecodeIterator(Process* process);
        BytecodeIterator(Process* process, Object* parent);

        void init(Function* function);

        Object* cur() const;
        Boolean* done() const;
        Object* next();

        BytecodeIterator* clone(Process* process, CloneCache& cache) override;

    private:
        std::shared_ptr<const Code> _code;
        size_t _i;
    };

#define X(name) NATIVE_FUNCTION(name);
    BYTECODE_NATIVES
    BYTECODE_ITERATOR_NATIVES
#undef X

    MODULE_INITIALIZATION_FUNC(init_bytecode_module);

} // namespace modules
} // namespace emerald

#endif // _EMERALD_MODULES_BYTECODE_H
