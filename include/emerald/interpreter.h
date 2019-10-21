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

namespace emerald {

    class Code;
    class ExecutionContext;
    class Object;
    class Module;

    class Interpreter {
    public:
        static Object* execute(ExecutionContext* context);
        static Object* execute_code(std::shared_ptr<const Code> code, ExecutionContext* context);
        static Object* execute_method(const std::string& name, const std::vector<Object*> args, ExecutionContext* context);
        static Object* execute_module(const std::string& module_name);
        static Module* import_module(const std::string& name, ExecutionContext* context);

    private:
        static Object* call_obj(Object* obj, const std::vector<Object*>& args, ExecutionContext* context);

        static Object* call_method(const std::string& name, size_t num_args, ExecutionContext* context);
        static Object* call_method1(const std::string& name, ExecutionContext* context) { return call_method(name, 1, context); }
        static Object* call_method2(const std::string& name, ExecutionContext* context) { return call_method(name, 2, context); }
        static Object* call_method(const std::string& name, const std::vector<Object*>& args, ExecutionContext* context);

        static Module* get_module(const std::string& name, bool& created, ExecutionContext* context);

        static Object* new_obj(bool explicit_parent, size_t num_props, ExecutionContext* context);
    };

} // namespace emerald

#endif // _EMERALD_INTERPRETER_H
