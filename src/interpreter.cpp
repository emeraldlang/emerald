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

#include "emerald/code.h"
#include "emerald/code_cache.h"
#include "emerald/execution_context.h"
#include "emerald/interpreter.h"
#include "emerald/iterutils.h"
#include "emerald/native_frame.h"
#include "emerald/module.h"
#include "emerald/objectutils.h"

namespace emerald {

    Object* Interpreter::execute(ExecutionContext* context) {
        Stack& stack = context->get_stack();
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
                    if (call_method0(current_frame.pop_ds(), magic_methods::boolean, context)->as_bool()) {
                        current_frame.set_instruction_pointer(instr.get_arg(0));
                    }
                    break;
                case OpCode::jmp_false:
                    if (!call_method0(current_frame.pop_ds(), magic_methods::boolean, context)->as_bool()) {
                        current_frame.set_instruction_pointer(instr.get_arg(0));
                    }
                    break;
                case OpCode::jmp_data:
                    if (current_frame.get_data_stack().size()) {
                        current_frame.set_instruction_pointer(instr.get_arg(0));
                    }
                    break;
                case OpCode::neg:
                    current_frame.push_ds(call_method0(current_frame.pop_ds(), magic_methods::neg, context));
                    break;
                case OpCode::log_neg:
                    current_frame.push_ds(BOOLEAN(
                        !call_method0(current_frame.pop_ds(), magic_methods::boolean, context)->as_bool()));
                    break;
                case OpCode::add:
                    current_frame.push_ds(call_method1(current_frame.pop_ds(), magic_methods::add, context));
                    break;
                case OpCode::sub:
                    current_frame.push_ds(call_method1(current_frame.pop_ds(), magic_methods::sub, context));  
                    break;
                case OpCode::mul:
                    current_frame.push_ds(call_method1(current_frame.pop_ds(), magic_methods::mul, context));
                    break;
                case OpCode::div:
                    current_frame.push_ds(call_method1(current_frame.pop_ds(), magic_methods::div, context));
                    break;
                case OpCode::mod:
                    current_frame.push_ds(call_method1(current_frame.pop_ds(), magic_methods::mod, context));
                    break;
                case OpCode::iadd:
                    current_frame.push_ds(call_method1(current_frame.pop_ds(), magic_methods::iadd, context));
                    break;
                case OpCode::isub:
                    current_frame.push_ds(call_method1(current_frame.pop_ds(), magic_methods::isub, context));
                    break;
                case OpCode::imul:
                    current_frame.push_ds(call_method1(current_frame.pop_ds(), magic_methods::imul, context));
                    break;
                case OpCode::idiv:
                    current_frame.push_ds(call_method1(current_frame.pop_ds(), magic_methods::idiv, context));
                    break;
                case OpCode::imod:
                    current_frame.push_ds(call_method1(current_frame.pop_ds(), magic_methods::imod, context));
                    break;
                case OpCode::eq:
                    current_frame.push_ds(call_method1(current_frame.pop_ds(), magic_methods::eq, context));
                    break;
                case OpCode::neq:
                    current_frame.push_ds(call_method1(current_frame.pop_ds(), magic_methods::neq, context));
                    break;
                case OpCode::lt:
                    current_frame.push_ds(call_method1(current_frame.pop_ds(), magic_methods::lt, context));
                    break;
                case OpCode::gt:
                    current_frame.push_ds(call_method1(current_frame.pop_ds(), magic_methods::gt, context));
                    break;
                case OpCode::lte:
                    current_frame.push_ds(call_method1(current_frame.pop_ds(), magic_methods::lte, context));
                    break;
                case OpCode::gte:
                    current_frame.push_ds(call_method1(current_frame.pop_ds(), magic_methods::gte, context));
                    break;
                case OpCode::bit_not:
                    current_frame.push_ds(call_method0(current_frame.pop_ds(), magic_methods::bit_not, context));
                    break;
                case OpCode::bit_or:
                    current_frame.push_ds(call_method1(current_frame.pop_ds(), magic_methods::bit_or, context));
                    break;
                case OpCode::bit_xor:
                    current_frame.push_ds(call_method1(current_frame.pop_ds(), magic_methods::bit_xor, context));
                    break;
                case OpCode::bit_and:
                    current_frame.push_ds(call_method1(current_frame.pop_ds(), magic_methods::bit_and, context));
                    break;
                case OpCode::bit_shl:
                    current_frame.push_ds(call_method1(current_frame.pop_ds(), magic_methods::bit_shl, context));
                    break;
                case OpCode::bit_shr:
                    current_frame.push_ds(call_method1(current_frame.pop_ds(), magic_methods::bit_shr, context));
                    break;
                case OpCode::str:
                    current_frame.push_ds(call_method0(current_frame.pop_ds(), magic_methods::str, context));
                    break;
                case OpCode::boolean:
                    current_frame.push_ds(call_method0(current_frame.pop_ds(), magic_methods::boolean, context));
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
                    current_frame.push_ds(call_obj(obj, receiver, args, context));
                    break;
                }
                case OpCode::ret: {
                    Object* ret = current_frame.pop_ds();
                    stack.pop_frame();
                    return ret;
                }
                case OpCode::new_obj:
                    current_frame.push_ds(new_obj(instr.get_arg(0), instr.get_arg(1), context));
                    break;
                case OpCode::init: {
                    Object* receiver = current_frame.pop_ds();
                    call_method(receiver, magic_methods::init, instr.get_arg(0), context);
                    current_frame.push_ds(receiver);
                    break;
                }
                case OpCode::new_func: {
                    std::shared_ptr<const Code> code = current_frame.get_code()->get_func(instr.get_arg(0));
                    Function* func = context->get_heap().allocate<Function>(
                        context,
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
                    current_frame.push_ds(context->get_native_objects().get_null());
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
                        throw context->get_heap().allocate<Exception>(context, fmt::format("no such property: {0}", key->as_str()));
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
                        throw context->get_heap().allocate<Exception>(
                            context,
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
                    current_frame.push_ds(call_method0(current_frame.pop_ds(), magic_methods::iter, context));
                    break;
                case OpCode::iter_cur:
                    current_frame.push_ds(call_method0(current_frame.peek_ds(), magic_methods::cur, context));
                    break;
                case OpCode::iter_done: {
                    Object* iter = current_frame.peek_ds();
                    Object* done = call_method0(iter, magic_methods::done, context);
                    if (done->as_bool()) {
                        current_frame.pop_ds();
                    }
                    current_frame.push_ds(done);
                    break;
                }
                case OpCode::iter_next:
                    current_frame.push_ds(call_method0(current_frame.peek_ds(), magic_methods::next, context));
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
                    std::cout << call_method0(current_frame.pop_ds(), magic_methods::str, context)->as_str() << std::endl;
                    break;
                case OpCode::import: {
                    const std::string& name = current_frame.get_code()->get_import_name(
                        instr.get_arg(0));
                    current_frame.push_ds(import_module(name, context));
                    break;
                }
                default:
                    break;
                }
            } catch (Object* exc) {
                if (!current_frame.has_catch_ip()) {
                    throw;
                }

                current_frame.set_instruction_pointer(current_frame.get_catch_ip());
                current_frame.pop_catch_ip();
                current_frame.push_ds(exc);
            }
        }

        stack.pop_frame();
        return context->get_native_objects().get_null();
    }

    Object* Interpreter::execute_code(std::shared_ptr<const Code> code, ExecutionContext* context) {
        context->get_stack().push_frame(NONE, code, context->get_stack().peek_globals());

        return execute(context);
    }

    Object* Interpreter::execute_method(Object* receiver, const std::string& name, const std::vector<Object*>& args, ExecutionContext* context) {
        return call_method(receiver, name, args, context);
    }

    Object* Interpreter::execute_module(const std::string& module_name) {
        ExecutionContext context;

        std::shared_ptr<Code> code = CodeCache::get_or_load_code(module_name);
        Module* entry_module = context.get_heap().allocate<Module>(&context, module_name, code);
        context.get_module_registry().add_module(entry_module);

        context.get_stack().push_frame(entry_module, entry_module->get_code(), entry_module);

        return execute(&context);    
    }

    Module* Interpreter::import_module(const std::string& name, ExecutionContext* context) {
        bool created;
        Module* module = get_module(name, created, context); 
        if (created && !module->is_native()) {
            context->get_stack().push_frame(module, module->get_code(), module);
            execute(context);
        }

        return module;
    }

    Object* Interpreter::call_obj(Object* obj, Object* receiver, const std::vector<Object*>& args, ExecutionContext* context) {
        Stack& stack = context->get_stack();
        if (Function* func = dynamic_cast<Function*>(obj)) {
            stack.push_frame(receiver, func->get_code(), func->get_globals());

            Stack::Frame& current_frame = stack.peek();
            for (Object* arg : iterutils::reverse(args)) {
                current_frame.push_ds(arg);
            }

            return execute(context);
        } else if (NativeFunction* func = dynamic_cast<NativeFunction*>(obj)) {
            NativeFrame frame(args);
            Heap& heap = context->get_heap();
            heap.add_root_source(&frame);
            Object* res = (*func)(receiver, &frame, context);
            heap.remove_root_source(&frame);
            return res;
        } else if (Object* prop = obj->get_property(magic_methods::call)) {
            return call_obj(prop, obj, args, context);
        } else {
            throw context->get_heap().allocate<Exception>(context, "object is not callable");
        }
    }

    Object* Interpreter::call_method(Object* receiver, const std::string& name, size_t num_args, ExecutionContext* context) {
        std::vector<Object*> args = context->get_stack().peek().pop_n_ds(num_args);
        return call_method(receiver, name, args, context);
    }

    Object* Interpreter::call_method(Object* receiver, const std::string& name, const std::vector<Object*>& args, ExecutionContext* context) {
        if (Object* method = receiver->get_property(name)) {
            return call_obj(method, receiver, args, context);
        } else {
            throw context->get_heap().allocate<Exception>(context, fmt::format("no such method: {0}", name));
        }
    }

    Module* Interpreter::get_module(const std::string& name, bool& created, ExecutionContext* context) {
        ModuleRegistry& registry = context->get_module_registry();
        if (registry.has_module(name)) {
            created = false;
            return registry.get_module(name);
        }

        Module* module;
        if (NativeModuleInitRegistry::has_module_init(name)) {
            module = NativeModuleInitRegistry::init_module(name, context);
        } else if (std::shared_ptr<Code> code = CodeCache::get_code(name)) {
            module = context->get_heap().allocate<Module>(context, name, code);
        } else {
            throw context->get_heap().allocate<Exception>(context, fmt::format("no such module: {0}", name));
        }

        registry.add_module(module);
        created = true;

        return module;
    }

    Object* Interpreter::new_obj(bool explicit_parent, size_t num_props, ExecutionContext* context) {
        Stack::Frame& current_frame = context->get_stack().peek();
        Object* receiver;
        if (explicit_parent) {
            receiver = current_frame.pop_ds();
        } else {
            receiver = context->get_native_objects().get_object_prototype();
        }

        Object* self = call_method0(receiver, magic_methods::clone, context);
        for (size_t i = 0; i < num_props; i++) {
            Object* key = current_frame.pop_ds();
            Object* val = current_frame.pop_ds();

            self->set_property(key->as_str(), val); 
        }

        return self;
    }

} // namespace emerald
