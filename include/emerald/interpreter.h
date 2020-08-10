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

#include "emerald/magic_methods.h"
#include "emerald/object.h"
#include "emerald/process.h"

namespace emerald {

    class Code;
    class Module;

    class Interpreter {
    public:
        static Object* execute(Process* process);
        template <class T>
        static T* execute_method(Object* receiver, const std::string& name, const std::vector<Object*>& args, Process* process);
        static Object* execute_module(const std::string& module_name, Process* process);
        static Module* import_module(const std::string& name, Process* process);
        template <class T>
        static T* create_obj(Object* parent, const std::vector<Object*>& args, Process* process);
        template <class T>
        static T* call_obj(Object* obj, Object* receiver, const std::vector<Object*>& args, Process* process);

    private:
        template <class T>
        static T* call_method(Object* receiver, const std::string& name, size_t num_args, Process* process);
        template <class T>
        static T* call_method0(Object* receiver, const std::string& name, Process* process) { return call_method<T>(receiver, name, 0, process); }
        template <class T>
        static T* call_method1(Object* receiver, const std::string& name, Process* process) { return call_method<T>(receiver, name, 1, process); }
        template <class T>
        static T* call_method2(Object* receiver, const std::string& name, Process* process) { return call_method<T>(receiver, name, 2, process); }
        template <class T>
        static T* call_method(Object* receiver, const std::string& name, const std::vector<Object*>& args, Process* process);

        static Module* get_module(const std::string& name, bool& created, Process* process);

        static Object* new_obj(bool explicit_parent, size_t num_props, Process* process);
    };

    template <class T>
    T* Interpreter::execute_method(Object* receiver, const std::string& name, const std::vector<Object*>& args, Process* process) {
        return call_method<T>(receiver, name, args, process);
    }

    template <class T>
    T* Interpreter::create_obj(Object* parent, const std::vector<Object*>& args, Process* process) {
        T* obj = Interpreter::execute_method<T>(
                parent,
                magic_methods::clone,
                {},
                process);
        Interpreter::execute_method<Null>(
            obj,
            magic_methods::init,
            args,
            process);

        return obj;
    }

    template <>
    Object* Interpreter::call_obj<Object>(Object* obj, Object* receiver, const std::vector<Object*>& args, Process* process);

    template <class T>
    T* Interpreter::call_obj(Object* obj, Object* receiver, const std::vector<Object*>& args, Process* process) {
        if (T* res = dynamic_cast<T*>(call_obj<Object>(obj, receiver, args, process))) {
            return res;
        }

        throw process->get_heap().allocate<Exception>(process, "");
    }

    template <class T>
    T* Interpreter::call_method(Object* receiver, const std::string& name, size_t num_args, Process* process) {
        std::vector<Object*> args = process->get_stack().peek().pop_n_ds(num_args);
        return call_method<T>(receiver, name, args, process);
    }

    template <class T>
    T* Interpreter::call_method(Object* receiver, const std::string& name, const std::vector<Object*>& args, Process* process) {
        if (Object* method = receiver->get_property(name)) {
            return call_obj<T>(method, receiver, args, process);
        } else {
            throw process->get_heap().allocate<Exception>(process, fmt::format("no such method: {0}", name));
        }
    }

} // namespace emerald

#endif // _EMERALD_INTERPRETER_H
