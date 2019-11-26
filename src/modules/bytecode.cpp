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

#include "emerald/magic_methods.h"
#include "emerald/module.h"
#include "emerald/modules/bytecode.h"
#include "emerald/native_variables.h"
#include "emerald/objectutils.h"
#include "emerald/process.h"

namespace emerald {
namespace modules {

    BytecodeIterator::BytecodeIterator(Process* process)
        : Object(process, OBJECT_PROTOTYPE),
        _code(nullptr),
        _i(0) {}

    BytecodeIterator::BytecodeIterator(Process* process, Object* parent)
        : Object(process, parent),
        _code(nullptr),
        _i(0) {}

    void BytecodeIterator::init(Function* function) {
        _code = function->get_code();
    }

    Object* BytecodeIterator::cur() const {
        Process* process = get_process();
        NativeStack::ScopedNativeFrame frame(process->get_native_stack().push_frame());

        size_t i;
        if (_i >= _code->get_num_instructions()) {
            i = _code->get_num_instructions() - 1;
        } else {
            i = _i;
        }

        const Code::Instruction& instr = (*_code)[i];
        Local<Object> obj = ALLOC_OBJECT();
        obj->set_property("op", ALLOC_NUMBER(instr.get_op()));
        obj->set_property("opname", ALLOC_STRING(OpCode::get_string(instr.get_op())));

        return obj.val();
    }

    Boolean* BytecodeIterator::done() const {
        return BOOLEAN_IN_CTX(_i >= _code->get_num_instructions(), get_process());
    }

    Object* BytecodeIterator::next() {
        _i++;
        return cur();
    }

    BytecodeIterator* BytecodeIterator::clone(Process* process, CloneCache& cache) {
        BytecodeIterator* clone = clone_impl<BytecodeIterator>(process, cache);
        clone->_code = _code;
        clone->_i = _i;
        return clone;
    }

    NATIVE_FUNCTION(bytecode_bytecode) {
        EXPECT_NUM_ARGS(1);

        CONVERT_ARG_TO(0, Function, function);

        return Interpreter::create_obj<BytecodeIterator>(
            frame->get_global("BytecodeIterator"),
            { function },
            process);
    }

    NATIVE_FUNCTION(bytecode_iterator_clone) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(BytecodeIterator, self);

        return process->get_heap().allocate<BytecodeIterator>(process, self);
    }

    NATIVE_FUNCTION(bytecode_iterator_init) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(BytecodeIterator, self);
        CONVERT_ARG_TO(0, Function, function);

        self->init(function);

        return NONE;
    }

    NATIVE_FUNCTION(bytecode_iterator_cur) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(BytecodeIterator, self);

        return self->cur();
    }

    NATIVE_FUNCTION(bytecode_iterator_done) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(BytecodeIterator, self);

        return self->done();
    }

    NATIVE_FUNCTION(bytecode_iterator_next) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(BytecodeIterator, self);

        return self->next();
    }

    MODULE_INITIALIZATION_FUNC(init_bytecode_module) {
        Process* process =  module->get_process();

        module->set_property("bytecode", ALLOC_NATIVE_FUNCTION(bytecode_bytecode));

        Local<BytecodeIterator> bytecode_iterator = process->get_heap().allocate<BytecodeIterator>(process);
        bytecode_iterator->set_property(magic_methods::clone, ALLOC_NATIVE_FUNCTION(bytecode_iterator_clone));
        bytecode_iterator->set_property(magic_methods::init, ALLOC_NATIVE_FUNCTION(bytecode_iterator_init));
        bytecode_iterator->set_property(magic_methods::cur, ALLOC_NATIVE_FUNCTION(bytecode_iterator_cur));
        bytecode_iterator->set_property(magic_methods::done, ALLOC_NATIVE_FUNCTION(bytecode_iterator_done));
        bytecode_iterator->set_property(magic_methods::next, ALLOC_NATIVE_FUNCTION(bytecode_iterator_next));
        module->set_property("BytecodeIterator", bytecode_iterator.val());
    }

} // namespace modules
} // namespace emerald
