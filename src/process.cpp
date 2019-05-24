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

#include <iostream>

#include "fmt/format.h"

#include "emerald/process.h"
#include "emerald/magic_methods.h"

namespace emerald {

    Process::Process(
            PID id, 
            std::shared_ptr<Code> code, 
            std::shared_ptr<Process> parent_process, 
            uint8_t priority, 
            uint16_t max_stack_size)
        : _id(id),
        _parent_process(parent_process),
        _priority(priority),
        _stack(Stack(max_stack_size)),
        _native_prototypes(&_heap),
        _state(State::start) {
        _stack.push_frame(code);

        _heap.add_root_source(&_data_stack);
        _heap.add_root_source(&_stack);
        _heap.add_root_source(&_native_prototypes);
    }

    Process::PID Process::get_id() const {
        return _id;
    }
    
    std::shared_ptr<const Process> Process::get_parent_process() const {
        return _parent_process;
    }

    std::shared_ptr<Process> Process::get_parent_process() {
        return _parent_process;
    }

    uint8_t Process::get_priority() const {
        return _priority;
    }

    Process::State Process::get_state() const {
        return _state;
    }

    void Process::set_suspended() {
        _heap.collect(); // TODO(zvp): Collect in allocate if has mem pressure
        _state = State::suspended;
    }

    void Process::set_running() {
        _state = State::running;
    }

    bool Process::is_suspended() const {
        return _state == State::suspended;
    }

    bool Process::is_running() const {
        return _state == State::running;
    }

    bool Process::is_terminated() const {
        return _state == State::terminated;
    }
    
    void Process::receive(Object* message) {
        // TODO(zvp): add lock free implementation of queue
        std::lock_guard<std::mutex> lock(_messages_mutex);
        _messages.push(message);
    }

    void Process::execute() {
        if (is_terminated()) return;

        // TODO(zvp): Add try-catch and set exception for current stack frame.
        dispatch();
    }

    void Process::dispatch() {
        while (!_stack.peek().has_instructions_left()) {
            _stack.pop_frame();
            if (_stack.empty()) {
                _heap.collect();
                _state = State::terminated;
                return;
            }
        }

        Stack::Frame& current_frame = _stack.peek();

        const Code::Instruction& instr = current_frame.get_next_instruction();
        current_frame.increment_instruction_pointer();

        switch (instr.get_op()) {
        case OpCode::nop:
            break;
        case OpCode::jmp:
            current_frame.set_instruction_pointer(instr.get_args()[0]);
            break;
        case OpCode::jmp_true: {
            Object* obj = _data_stack.pop();
            if (obj->as_bool()) {
                current_frame.set_instruction_pointer(instr.get_args()[0]);
            }
            break;
        }
        case OpCode::jmp_false: {
            Object* obj = _data_stack.pop();
            if (!obj->as_bool()) {
                current_frame.set_instruction_pointer(instr.get_args()[0]);
            }
            break;
        }
        case OpCode::neg:
            execute_mm1(magic_methods::neg);
            break;
        case OpCode::add:
            execute_mm2(magic_methods::add);
            break;
        case OpCode::sub:
            execute_mm2(magic_methods::sub);  
            break;
        case OpCode::mul:
            execute_mm2(magic_methods::mul);
            break;
        case OpCode::div:
            execute_mm2(magic_methods::div);
            break;
        case OpCode::mod:
            execute_mm2(magic_methods::mod);
            break;
        case OpCode::prefix_inc:
            execute_mm1(magic_methods::prefix_inc);
            break;
        case OpCode::prefix_dec:
            execute_mm1(magic_methods::prefix_dec);
            break;
        case OpCode::postfix_inc:
            execute_mm1(magic_methods::postfix_inc);
            break;
        case OpCode::postfix_dec:
            execute_mm1(magic_methods::postfix_dec);
            break;
        case OpCode::eq:
            execute_mm2(magic_methods::eq);
            break;
        case OpCode::neq:
            execute_mm2(magic_methods::neq);
            break;
        case OpCode::lt:
            execute_mm2(magic_methods::lt);
            break;
        case OpCode::gt:
            execute_mm2(magic_methods::gt);
            break;
        case OpCode::lte:
            execute_mm2(magic_methods::lte);
            break;
        case OpCode::gte:
            execute_mm2(magic_methods::gte);
            break;
        case OpCode::bit_not:
            execute_mm1(magic_methods::bit_not);
            break;
        case OpCode::bit_or:
            execute_mm2(magic_methods::bit_or);
            break;
        case OpCode::bit_xor:
            execute_mm2(magic_methods::bit_xor);
            break;
        case OpCode::bit_and:
            execute_mm2(magic_methods::bit_and);
            break;
        case OpCode::bit_shl:
            execute_mm2(magic_methods::bit_shl);
            break;
        case OpCode::bit_shr:
            execute_mm2(magic_methods::bit_shr);
            break;
        case OpCode::str:
            execute_mm1(magic_methods::str, [this](Object* obj){
                return this->allocate_string(obj->as_str());
            });
            break;
        case OpCode::boolean:
            execute_mm1(magic_methods::boolean, [this](Object* obj){
                return this->_heap.allocate<Boolean>(obj->as_bool());
            });
            break;
        case OpCode::call: {
            Object* obj = _data_stack.pop();
            std::vector<Object*> args = pop_n_from_stack(instr.get_args()[0]);
            call_obj(obj, args);
            break;
        }
        case OpCode::ret:
            _stack.pop_frame();
            break;
        case OpCode::new_obj: {
            const std::vector<uint64_t>& args = instr.get_args();
            _data_stack.push(new_obj(args[0], args[1]));
            break;
        }
        case OpCode::new_obj_and_init: {
            const std::vector<uint64_t>& args = instr.get_args();
            Object* self = new_obj(args[0], args[1]);

            std::vector<Object*> mm_args({ self });
            pop_n_from_stack(mm_args, args[2]);
            execute_mm(magic_methods::init, mm_args);

            _data_stack.push(self);
            break;
        }
        case OpCode::new_func: {
            std::shared_ptr<const Code> code = current_frame.get_code()->get_func(instr.get_args()[0]);
            Function* func = _heap.allocate<Function>(code);
            _data_stack.push(func);
            break;
        }
        case OpCode::new_num: {
            double value = current_frame.get_code()->get_num_constant(instr.get_args()[0]);
            Number* num = allocate_number(value);
            _data_stack.push(num);
            break;
        }
        case OpCode::new_str: {
            const std::string& value = current_frame.get_code()->get_str_constant(instr.get_args()[0]);
            String* str = allocate_string(value);
            _data_stack.push(str);
            break;
        }
        case OpCode::new_boolean: {
            bool value = instr.get_args()[0];
            Boolean* boolean = _heap.allocate<Boolean>(value);
            _data_stack.push(boolean);
            break;
        }
        case OpCode::new_arr: {
            Array* array = allocate_array();
            for (size_t i = 0; i < instr.get_args()[0]; i++) {
                array->get_value().push_back(_data_stack.pop());
            }
            _data_stack.push(array);
            break;
        }
        case OpCode::get_prop: {
            Object* obj = _data_stack.pop();
            Object* key = _data_stack.pop();
            if (Object* val = obj->get_property(key->as_str())) {
                _data_stack.push(val);
            } else {
                 throw _heap.allocate<Exception>("");
            }
            break;
        }
        case OpCode::has_prop: {
            Object* obj = _data_stack.pop();
            Object* key = _data_stack.pop();
            _data_stack.push(_heap.allocate<Boolean>(obj->has_property(key->as_str())));
            break;
        }
        case OpCode::set_prop: {
            Object* obj = _data_stack.pop();
            Object* key = _data_stack.pop();
            Object* val = _data_stack.pop();
            obj->set_property(key->as_str(), val);
            break;
        }
        case OpCode::get_parent: {
            Object* obj = _data_stack.pop();
            _data_stack.push(obj->get_parent());
            break;
        }
        case OpCode::ldloc: {
            const std::string& name = current_frame.get_code()->get_local_name(
                instr.get_args()[0]);
            _data_stack.push(current_frame.get_local(name));
            break;
        }
        case OpCode::stloc: {
            const std::string& name = current_frame.get_code()->get_local_name(
                instr.get_args()[0]);
            current_frame.set_local(name, _data_stack.pop());
            break;
        }
        case OpCode::print: {
            Object* obj = _data_stack.pop();
            std::cout << obj->as_str() << std::endl;
            break;
        }
        case OpCode::import: 
            break;
        default:
            break;
        }
    }

    void Process::execute_mm(
        const std::string& magic_method,
        const std::vector<Object*>& args,
        std::function<Object*(Object*)> on_missing) {
        if (Object* prop = args[0]->get_property(magic_method)) {
            call_obj(prop, args);
        } else if (on_missing) {
            _data_stack.push(on_missing(prop));
        } else {
            throw _heap.allocate<Exception>(fmt::format("unsupported method: {0}", magic_method));
        }
    }

    void Process::execute_mm(
        const std::string& magic_method,
        size_t nargs,
        std::function<Object*(Object*)> on_missing) {
        std::vector<Object*> args = pop_n_from_stack(nargs);
        execute_mm(magic_method, args, on_missing);
    }

    Object* Process::new_obj(bool explicit_parent, size_t num_props) {
        if (!explicit_parent) {
            _data_stack.push(_native_prototypes.get_object_prototype());
        }

        execute_mm1(magic_methods::clone);

        Object* self = _data_stack.pop();
        for (size_t i = 0; i < num_props; i++) {
            Object* key = _data_stack.pop();
            Object* val = _data_stack.pop();
            self->set_property(key->as_str(), val);
        }

        return self;
    }

    void Process::call_obj(Object* obj, const std::vector<Object*>& args) {
        if (Function* func = dynamic_cast<Function*>(obj)) {
            for (Object* arg : args) {
                _data_stack.push(arg);
            }

            _stack.push_frame(func->get_code());
        } else if (NativeFunction* func = dynamic_cast<NativeFunction*>(obj)) {
            _data_stack.push((*func)(&_heap, &_native_prototypes, args));
        } else if (Object* func = obj->get_property(magic_methods::call)) {
            call_obj(func, args);
        }
    }

    void Process::pop_n_from_stack(std::vector<Object*>& vec, size_t n) {
        for (size_t i = 0; i < n; i++) {
            vec.push_back(_data_stack.pop());
        }
    }
    
    std::vector<Object*> Process::pop_n_from_stack(size_t n) {
        std::vector<Object*> vec;
        pop_n_from_stack(vec, n);
        return vec;   
    }

    bool Process::ProcessPriorityComparator::operator()(const Process& lhs, 
        const Process& rhs) const {
        return lhs.get_priority() < rhs.get_priority();
    }

    bool Process::SharedProcessPriorityComparator::operator()(std::shared_ptr<const Process> lhs, 
        std::shared_ptr<const Process> rhs) const {
        return lhs->get_priority() < rhs->get_priority();
    }

} // namespace emerald
