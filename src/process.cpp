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

#include <filesystem>
#include <iostream>

#include "fmt/format.h"

#include "emerald/process.h"
#include "emerald/code_cache.h"
#include "emerald/magic_methods.h"
#include "emerald/strutils.h"

namespace emerald {

    Process::Process(
            PID id, 
            const std::string& entry_module_name,
            std::shared_ptr<Process> parent_process,
            uint8_t priority,
            uint16_t max_stack_size)
        : _id(id),
        _parent_process(parent_process),
        _priority(priority),
        _stack(Stack(max_stack_size)),
        _native_prototypes(&_heap),
        _state(State::ready) {
        _heap.add_root_source(&_module_registry);
        _heap.add_root_source(&_data_stack);
        _heap.add_root_source(&_stack);

        std::shared_ptr<Code> code = CodeCache::get_or_load_code(entry_module_name);
        Module* entry_module = _heap.allocate<Module>(entry_module_name, code);
        _module_registry.add_module(entry_module);
        _globals.push(entry_module);
        _stack.push_frame(entry_module->get_code());
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

    void Process::set_ready() {
        _state = State::ready;
    }

    void Process::set_running() {
        _state = State::running;
    }

    bool Process::is_ready() const {
        return _state == State::ready;
    }

    bool Process::is_running() const {
        return _state == State::running;
    }

    bool Process::is_waiting() const {
        return _state == State::waiting;
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
        Stack::Frame& current_frame = _stack.peek();
        while (!current_frame.has_instructions_left()) {
            bool is_top_level = current_frame.get_code()->is_top_level();
            _stack.pop_frame();
            if (_stack.empty()) {
                _heap.collect();
                _state = State::terminated;
                return;
            } else if (is_top_level) {
                Module* imported_module = _globals.top();
                _globals.pop();
                _data_stack.push(imported_module);
            }
        }

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
        case OpCode::iadd:
            execute_mm2(magic_methods::iadd);
            break;
        case OpCode::isub:
            execute_mm2(magic_methods::isub);
            break;
        case OpCode::imul:
            execute_mm2(magic_methods::imul);
            break;
        case OpCode::idiv:
            execute_mm2(magic_methods::idiv);
            break;
        case OpCode::imod:
            execute_mm2(magic_methods::imod);
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
        case OpCode::init: {
            Object* self = _data_stack.pop();

            std::vector<Object*> mm_args({ self });
            pop_n_from_stack(mm_args, instr.get_args()[0]);
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
                 throw _heap.allocate<Exception>(fmt::format("no such property: {0}", key->as_str()));
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
        case OpCode::ldgbl: {
            const std::string& name = current_frame.get_code()->get_global_name(
                instr.get_args()[0]);
            _data_stack.push(get_global(name));
            break;
        }
        case OpCode::stgbl: {
            const std::string& name = current_frame.get_code()->get_global_name(
                instr.get_args()[0]);
            set_global(name, _data_stack.pop());
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
        case OpCode::import: {
            const std::string& import_name = current_frame.get_code()->get_import_name(
                instr.get_args()[0]);
            bool created;
            Module* module = get_module(import_name, created);
            if (created && !module->is_native()) {
                _globals.push(module);
                _stack.push_frame(module->get_code());
            } else {
                _data_stack.push(module);
            }
            break;
        }
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
            // This is not necessary, complicates compiler logic
            for (Object* arg : args) {
                _data_stack.push(arg);
            }

            _stack.push_frame(func->get_code());
        } else if (NativeFunction* func = dynamic_cast<NativeFunction*>(obj)) {
            _data_stack.push((*func)(&_heap, &_native_prototypes, args));
        } else if (Object* func = obj->get_property(magic_methods::call)) {
            call_obj(func, args);
        } else {
            throw _heap.allocate<Exception>("object is not callable");
        }
    }

    Module* Process::get_module(const std::string& name, bool& created) {
        if (_module_registry.has_module(name)) {
            created = false;
            return _module_registry.get_module(name);
        }

        Module* module = nullptr;
        if (NativeModuleInitRegistry::is_module_init_registered(name)) {
            module = NativeModuleInitRegistry::init_module(name, &_heap, &_native_prototypes);
        } else if (std::shared_ptr<Code> code = CodeCache::get_code(name)) {
            module = _heap.allocate<Module>(name, code);
        } else {
            throw _heap.allocate<Exception>(fmt::format("no such module: {0}", name));
        }

        _module_registry.add_module(module);
        created = true;

        return module;
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
