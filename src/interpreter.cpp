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

#include "emerald/code.h"
#include "emerald/code_cache.h"
#include "emerald/interpreter.h"
#include "emerald/iterutils.h"
#include "emerald/native_frame.h"
#include "emerald/module.h"
#include "emerald/objectutils.h"

namespace emerald {

    Object* Interpreter::execute(Process* process) {
        Stack& stack = process->get_stack();
        Stack::Frame& current_frame = stack.peek();
        while (current_frame.has_instructions_left()) {
            try {
                const Code::Instruction& instr = current_frame.get_next_instruction();
                current_frame.increment_instruction_pointer();

                switch (instr.get_op()) {
                case OpCode::nop:
                    break;
                case OpCode::jmp:
                    current_frame.set_instruction_pointer(instr.get_arg(0));
                    break;
                case OpCode::jmp_true:
                    if (call_method0<Boolean>(current_frame.pop_ds(), magic_methods::boolean, process)->get_native_value()) {
                        current_frame.set_instruction_pointer(instr.get_arg(0));
                    }
                    break;
                case OpCode::jmp_true_or_pop:
                    if (call_method0<Boolean>(current_frame.peek_ds(), magic_methods::boolean, process)->get_native_value()) {
                        current_frame.set_instruction_pointer(instr.get_arg(0));
                    } else {
                        current_frame.pop_ds();
                    }
                    break;
                case OpCode::jmp_false:
                    if (!call_method0<Boolean>(current_frame.pop_ds(), magic_methods::boolean, process)->get_native_value()) {
                        current_frame.set_instruction_pointer(instr.get_arg(0));
                    }
                    break;
                case OpCode::jmp_false_or_pop:
                    if (!call_method0<Boolean>(current_frame.peek_ds(), magic_methods::boolean, process)->get_native_value()) {
                        current_frame.set_instruction_pointer(instr.get_arg(0));
                    } else {
                        current_frame.pop_ds();
                    }
                    break;
                case OpCode::jmp_data:
                    if (current_frame.get_data_stack().size()) {
                        current_frame.set_instruction_pointer(instr.get_arg(0));
                    }
                    break;
                case OpCode::neg:
                    current_frame.push_ds(call_method0<Object>(current_frame.pop_ds(), magic_methods::neg, process));
                    break;
                case OpCode::log_neg:
                    current_frame.push_ds(BOOLEAN(
                        !call_method0<Boolean>(
                            current_frame.pop_ds(),
                            magic_methods::boolean,
                            process
                        )->get_native_value()));
                    break;
                case OpCode::add:
                    current_frame.push_ds(call_method1<Object>(current_frame.pop_ds(), magic_methods::add, process));
                    break;
                case OpCode::sub:
                    current_frame.push_ds(call_method1<Object>(current_frame.pop_ds(), magic_methods::sub, process));  
                    break;
                case OpCode::mul:
                    current_frame.push_ds(call_method1<Object>(current_frame.pop_ds(), magic_methods::mul, process));
                    break;
                case OpCode::div:
                    current_frame.push_ds(call_method1<Object>(current_frame.pop_ds(), magic_methods::div, process));
                    break;
                case OpCode::mod:
                    current_frame.push_ds(call_method1<Object>(current_frame.pop_ds(), magic_methods::mod, process));
                    break;
                case OpCode::iadd:
                    current_frame.push_ds(call_method1<Object>(current_frame.pop_ds(), magic_methods::iadd, process));
                    break;
                case OpCode::isub:
                    current_frame.push_ds(call_method1<Object>(current_frame.pop_ds(), magic_methods::isub, process));
                    break;
                case OpCode::imul:
                    current_frame.push_ds(call_method1<Object>(current_frame.pop_ds(), magic_methods::imul, process));
                    break;
                case OpCode::idiv:
                    current_frame.push_ds(call_method1<Object>(current_frame.pop_ds(), magic_methods::idiv, process));
                    break;
                case OpCode::imod:
                    current_frame.push_ds(call_method1<Object>(current_frame.pop_ds(), magic_methods::imod, process));
                    break;
                case OpCode::eq:
                    current_frame.push_ds(call_method1<Object>(current_frame.pop_ds(), magic_methods::eq, process));
                    break;
                case OpCode::neq:
                    current_frame.push_ds(call_method1<Object>(current_frame.pop_ds(), magic_methods::neq, process));
                    break;
                case OpCode::lt:
                    current_frame.push_ds(call_method1<Object>(current_frame.pop_ds(), magic_methods::lt, process));
                    break;
                case OpCode::gt:
                    current_frame.push_ds(call_method1<Object>(current_frame.pop_ds(), magic_methods::gt, process));
                    break;
                case OpCode::lte:
                    current_frame.push_ds(call_method1<Object>(current_frame.pop_ds(), magic_methods::lte, process));
                    break;
                case OpCode::gte:
                    current_frame.push_ds(call_method1<Object>(current_frame.pop_ds(), magic_methods::gte, process));
                    break;
                case OpCode::bit_not:
                    current_frame.push_ds(call_method0<Object>(current_frame.pop_ds(), magic_methods::bit_not, process));
                    break;
                case OpCode::bit_or:
                    current_frame.push_ds(call_method1<Object>(current_frame.pop_ds(), magic_methods::bit_or, process));
                    break;
                case OpCode::bit_xor:
                    current_frame.push_ds(call_method1<Object>(current_frame.pop_ds(), magic_methods::bit_xor, process));
                    break;
                case OpCode::bit_and:
                    current_frame.push_ds(call_method1<Object>(current_frame.pop_ds(), magic_methods::bit_and, process));
                    break;
                case OpCode::bit_shl:
                    current_frame.push_ds(call_method1<Object>(current_frame.pop_ds(), magic_methods::bit_shl, process));
                    break;
                case OpCode::bit_shr:
                    current_frame.push_ds(call_method1<Object>(current_frame.pop_ds(), magic_methods::bit_shr, process));
                    break;
                case OpCode::str:
                    current_frame.push_ds(call_method0<String>(current_frame.pop_ds(), magic_methods::str, process));
                    break;
                case OpCode::boolean:
                    current_frame.push_ds(call_method0<Boolean>(current_frame.pop_ds(), magic_methods::boolean, process));
                    break;
                case OpCode::call: {
                    Object* obj = current_frame.pop_ds();
                    Object* receiver;
                    if (instr.get_arg(0)) {
                        receiver = current_frame.pop_ds();
                    } else {
                        receiver = current_frame.get_globals();
                    }
                    std::vector<Object*> args = current_frame.pop_n_ds(instr.get_arg(1));
                    current_frame.push_ds(call_obj<Object>(obj, receiver, args, process));
                    break;
                }
                case OpCode::ret: {
                    Object* ret = current_frame.pop_ds();
                    stack.pop_frame();
                    return ret;
                }
                case OpCode::new_obj:
                    current_frame.push_ds(new_obj(instr.get_arg(0), instr.get_arg(1), process));
                    break;
                case OpCode::init: {
                    Object* receiver = current_frame.pop_ds();
                    call_method<Object>(receiver, magic_methods::init, instr.get_arg(0), process);
                    current_frame.push_ds(receiver);
                    break;
                }
                case OpCode::new_func: {
                    std::shared_ptr<const Code> code = current_frame.get_code()->get_func(instr.get_arg(0));
                    Function* func = process->get_heap().allocate<Function>(
                        process,
                        code,
                        current_frame.get_globals());
                    current_frame.push_ds(func);
                    break;
                }
                case OpCode::new_num: {
                    double value = current_frame.get_code()->get_num_constant(instr.get_arg(0));
                    Number* num = ALLOC_NUMBER(value);
                    current_frame.push_ds(num);
                    break;
                }
                case OpCode::new_str: {
                    const std::string& value = current_frame.get_code()->get_str_constant(instr.get_arg(0));
                    String* str = ALLOC_STRING(value);
                    current_frame.push_ds(str);
                    break;
                }
                case OpCode::new_boolean: {
                    bool value = instr.get_arg(0);
                    Boolean* boolean = BOOLEAN(value);
                    current_frame.push_ds(boolean);
                    break;
                }
                case OpCode::new_arr: {
                    Array* array = ALLOC_EMPTY_ARRAY();
                    for (size_t i = 0; i < instr.get_arg(0); i++) {
                        array->push(current_frame.pop_ds());
                    }
                    current_frame.push_ds(array);
                    break;
                }
                case OpCode::null: {
                    current_frame.push_ds(NONE);
                    break;
                }
                case OpCode::get_prop: {
                    Object* obj = current_frame.pop_ds();
                    Object* key = current_frame.pop_ds();
                    if (Object* val = obj->get_property(key->as_str())) {
                        if (instr.get_arg(0)) {
                            current_frame.push_ds(obj);
                        }
                        current_frame.push_ds(val);
                    } else {
                        throw process->get_heap().allocate<Exception>(process, fmt::format("no such property: {0}", key->as_str()));
                    }
                    break;
                }
                case OpCode::set_prop: {
                    Object* obj = current_frame.pop_ds();
                    Object* key = current_frame.pop_ds();
                    Object* val = current_frame.pop_ds();
                    if (instr.get_arg(0)) {
                        current_frame.push_ds(obj);
                    }
                    if (!obj->set_property(key->as_str(), val)) {
                        throw process->get_heap().allocate<Exception>(
                            process,
                            fmt::format("could not set property: {0} of {1}", key->as_str(), obj->as_str()));
                    }
                    break;
                }
                case OpCode::self:
                    current_frame.push_ds(current_frame.get_receiver());
                    break;
                case OpCode::enter_try:
                    current_frame.push_catch_ip(instr.get_arg(0));
                    break;
                case OpCode::exit_try:
                    current_frame.pop_catch_ip();
                    current_frame.set_instruction_pointer(instr.get_arg(0));
                    break;
                case OpCode::throw_exc:
                    throw current_frame.pop_ds();
                case OpCode::get_iter:
                    current_frame.push_ds(call_method0<Object>(current_frame.pop_ds(), magic_methods::iter, process));
                    break;
                case OpCode::iter_cur:
                    current_frame.push_ds(call_method0<Object>(current_frame.peek_ds(), magic_methods::cur, process));
                    break;
                case OpCode::iter_done: {
                    Object* iter = current_frame.peek_ds();
                    Boolean* done = call_method0<Boolean>(iter, magic_methods::done, process);
                    if (done->get_native_value()) {
                        current_frame.pop_ds();
                    }
                    current_frame.push_ds(done);
                    break;
                }
                case OpCode::iter_next:
                    current_frame.push_ds(call_method0<Object>(current_frame.peek_ds(), magic_methods::next, process));
                    break;
                case OpCode::ldgbl: {
                    const std::string& name = current_frame.get_code()->get_global_name(
                        instr.get_arg(0));
                    Object* global = current_frame.get_global(name);
                    if (global == nullptr) global = NONE;
                    current_frame.push_ds(global);
                    break;
                }
                case OpCode::stgbl: {
                    const std::string& name = current_frame.get_code()->get_global_name(
                        instr.get_arg(0));
                    Object* val = current_frame.pop_ds();
                    current_frame.set_global(name, val);
                    break;
                }
                case OpCode::ldloc: {
                    const std::string& name = current_frame.get_code()->get_local_name(
                        instr.get_arg(0));
                    Object* local = current_frame.get_local(name);
                    if (local == nullptr) local = NONE;
                    current_frame.push_ds(local);
                    break;
                }
                case OpCode::stloc: {
                    const std::string& name = current_frame.get_code()->get_local_name(
                        instr.get_arg(0));
                    current_frame.set_local(name, current_frame.pop_ds());
                    break;
                }
                case OpCode::print:
                    std::cout << call_method0<String>(current_frame.pop_ds(), magic_methods::str, process)->get_native_value() << std::endl;
                    break;
                case OpCode::import: {
                    const std::string& name = current_frame.get_code()->get_import_name(
                        instr.get_arg(0));
                    current_frame.push_ds(import_module(name, process));
                    break;
                }
                default:
                    break;
                }
            } catch (Object* exc) {
                if (!current_frame.has_catch_ip()) {
                    stack.pop_frame();
                    throw;
                }

                current_frame.set_instruction_pointer(current_frame.get_catch_ip());
                current_frame.pop_catch_ip();
                current_frame.push_ds(exc);
            }
        }

        stack.pop_frame();
        return NONE;
    }

    Object* Interpreter::execute_function(Function* function, Process* process) {
        process->get_stack().push_frame(
            function->get_globals(),
            function->get_code(),
            function->get_globals());

        return execute(process);
    }

    Object* Interpreter::execute_module(const std::string& module_name) {
        Process* process = Processes::create_process();

        std::shared_ptr<Code> code = CodeCache::get_or_load_code(module_name);
        Module* entry_module = process->get_heap().allocate<Module>(process, module_name, code);
        process->get_module_registry().add_module(entry_module);

        process->get_stack().push_frame(entry_module, entry_module->get_code(), entry_module);

        return execute(process);    
    }

    Module* Interpreter::import_module(const std::string& name, Process* process) {
        bool created;
        Module* module = get_module(name, created, process); 
        if (created && !module->is_native()) {
            process->get_stack().push_frame(module, module->get_code(), module);
            execute(process);
        }

        return module;
    }

    template <>
    Object* Interpreter::call_obj<Object>(Object* obj, Object* receiver, const std::vector<Object*>& args, Process* process) {
        Stack& stack = process->get_stack();
        if (Function* func = dynamic_cast<Function*>(obj)) {
            stack.push_frame(receiver, func->get_code(), func->get_globals());

            Stack::Frame& current_frame = stack.peek();
            for (Object* arg : iterutils::reverse(args)) {
                current_frame.push_ds(arg);
            }

            return execute(process);
        } else if (NativeFunction* func = dynamic_cast<NativeFunction*>(obj)) {
            NativeFrame frame(args, func->get_globals());
            Heap& heap = process->get_heap();
            heap.add_root_source(&frame);
            Object* res = (*func)(receiver, &frame, process);
            heap.remove_root_source(&frame);
            return res;
        } else if (Object* prop = obj->get_property(magic_methods::call)) {
            return call_obj<Object>(prop, obj, args, process);
        } else {
            throw process->get_heap().allocate<Exception>(process, "object is not callable");
        }
    }

    Module* Interpreter::get_module(const std::string& name, bool& created, Process* process) {
        ModuleRegistry& registry = process->get_module_registry();
        if (registry.has_module(name)) {
            created = false;
            return registry.get_module(name);
        }

        Module* module;
        if (NativeModuleInitRegistry::has_module_init(name)) {
            module = NativeModuleInitRegistry::init_module(name, process);
        } else if (std::shared_ptr<Code> code = CodeCache::get_code(name)) {
            module = process->get_heap().allocate<Module>(process, name, code);
        } else {
            throw process->get_heap().allocate<Exception>(process, fmt::format("no such module: {0}", name));
        }

        registry.add_module(module);
        created = true;

        return module;
    }

    Object* Interpreter::new_obj(bool explicit_parent, size_t num_props, Process* process) {
        Stack::Frame& current_frame = process->get_stack().peek();
        Object* receiver;
        if (explicit_parent) {
            receiver = current_frame.pop_ds();
        } else {
            receiver = OBJECT_PROTOTYPE;
        }

        Object* self = call_method0<Object>(receiver, magic_methods::clone, process);
        for (size_t i = 0; i < num_props; i++) {
            Object* key = current_frame.pop_ds();
            Object* val = current_frame.pop_ds();

            self->set_property(key->as_str(), val); 
        }

        return self;
    }

} // namespace emerald
