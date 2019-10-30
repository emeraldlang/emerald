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

#ifndef _EMERALD_INTERPRETER_H
#define _EMERALD_INTERPRETER_H

#include <string>
#include <vector>

#include "fmt/format.h"

#include "emerald/execution_context.h"
#include "emerald/magic_methods.h"
#include "emerald/object.h"

namespace emerald {

    class Code;
    class Module;

    class Interpreter {
    public:
        static Object* execute(ExecutionContext* context);
        static Object* execute_code(std::shared_ptr<const Code> code, ExecutionContext* context);
        template <class T>
        static T* execute_method(Object* receiver, const std::string& name, const std::vector<Object*>& args, ExecutionContext* context);
        static Object* execute_module(const std::string& module_name);
        static Module* import_module(const std::string& name, ExecutionContext* context);
        template <class T>
        static T* create_obj(Object* parent, const std::vector<Object*>& args, ExecutionContext* context);

    private:
        template <class T>
        static T* call_obj(Object* obj, Object* receiver, const std::vector<Object*>& args, ExecutionContext* context);

        template <class T>
        static T* call_method(Object* receiver, const std::string& name, size_t num_args, ExecutionContext* context);
        template <class T>
        static T* call_method0(Object* receiver, const std::string& name, ExecutionContext* context) { return call_method<T>(receiver, name, 0, context); }
        template <class T>
        static T* call_method1(Object* receiver, const std::string& name, ExecutionContext* context) { return call_method<T>(receiver, name, 1, context); }
        template <class T>
        static T* call_method2(Object* receiver, const std::string& name, ExecutionContext* context) { return call_method<T>(receiver, name, 2, context); }
        template <class T>
        static T* call_method(Object* receiver, const std::string& name, const std::vector<Object*>& args, ExecutionContext* context);

        static Module* get_module(const std::string& name, bool& created, ExecutionContext* context);

        static Object* new_obj(bool explicit_parent, size_t num_props, ExecutionContext* context);
    };

    template <class T>
    T* Interpreter::execute_method(Object* receiver, const std::string& name, const std::vector<Object*>& args, ExecutionContext* context) {
        return call_method<T>(receiver, name, args, context);
    }

    template <class T>
    T* Interpreter::create_obj(Object* parent, const std::vector<Object*>& args, ExecutionContext* context) {
        T* obj = Interpreter::execute_method<T>(
                parent,
                magic_methods::clone,
                {},
                context);
        Interpreter::execute_method<Null>(
            obj,
            magic_methods::init,
            args,
            context);

        return obj;
    }

    template <>
    Object* Interpreter::call_obj<Object>(Object* obj, Object* receiver, const std::vector<Object*>& args, ExecutionContext* context);

    template <class T>
    T* Interpreter::call_obj(Object* obj, Object* receiver, const std::vector<Object*>& args, ExecutionContext* context) {
        if (T* res = dynamic_cast<T*>(call_obj<Object>(obj, receiver, args, context))) {
            return res;
        }

        throw context->get_heap().allocate<Exception>(context, "");
    }

    template <class T>
    T* Interpreter::call_method(Object* receiver, const std::string& name, size_t num_args, ExecutionContext* context) {
        std::vector<Object*> args = context->get_stack().peek().pop_n_ds(num_args);
        return call_method<T>(receiver, name, args, context);
    }

    template <class T>
    T* Interpreter::call_method(Object* receiver, const std::string& name, const std::vector<Object*>& args, ExecutionContext* context) {
        if (Object* method = receiver->get_property(name)) {
            return call_obj<T>(method, receiver, args, context);
        } else {
            throw context->get_heap().allocate<Exception>(context, fmt::format("no such method: {0}", name));
        }
    }

} // namespace emerald

#endif // _EMERALD_INTERPRETER_H
