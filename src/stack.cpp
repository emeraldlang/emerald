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

#include "emerald/check.h"
#include "emerald/module.h"
#include "emerald/object.h"
#include "emerald/stack.h"

namespace emerald {

    Stack::Stack(uint16_t max_size)
        : _max_size(max_size) {}

    uint16_t Stack::max_size() const {
        return _max_size;
    }

    size_t Stack::size() const {
        return _stack.size();
    }

    bool Stack::empty() const {
        return _stack.empty();
    }

    const Stack::Frame& Stack::peek() const {
        CHECK_THROW_LOGIC_ERROR(!_stack.empty(), "cannot peek an empty stack");

        return _stack.back();
    }

    Stack::Frame& Stack::peek() {
        CHECK_THROW_LOGIC_ERROR(!_stack.empty(), "cannot peek an empty stack");

        return _stack.back();
    }

    bool Stack::pop_frame() {
        if (_stack.empty()) return false;

        _stack.pop_back();
        return true;
    }

    void Stack::push_frame(std::shared_ptr<const Code> code, Module* globals) {
        _stack.emplace_back(code, globals);
    }

    const Module* Stack::peek_globals() const {
        CHECK_THROW_LOGIC_ERROR(!_stack.empty(), "cannot peek an empty stack");

        return _stack.back().get_globals();
    }

    Module* Stack::peek_globals() {
        CHECK_THROW_LOGIC_ERROR(!_stack.empty(), "cannot peek an empty stack");

        return _stack.back().get_globals();
    }

    std::vector<HeapManaged*> Stack::get_roots() {
        std::vector<HeapManaged*> roots;
        for (Frame& frame : _stack) {
            for (std::pair<std::string, Object*> local : frame.get_locals()) {
                roots.push_back(local.second);
            }

            roots.push_back(frame.get_globals());

            for (Object* obj : frame.get_data_stack()) {
                roots.push_back(obj);
            }
        }

        return roots;
    }

    Stack::Frame::Frame(std::shared_ptr<const Code> code, Module* globals)
        : _code(code), 
        _ip(0),
        _globals(globals) {}

    std::shared_ptr<const Code> Stack::Frame::get_code() const {
        return _code;
    }

    size_t Stack::Frame::get_instruction_pointer() const {
        return _ip;
    }

    void Stack::Frame::increment_instruction_pointer() {
        _ip++;
    }

    void Stack::Frame::set_instruction_pointer(size_t ip) {
        _ip = ip;
    }

    bool Stack::Frame::has_instructions_left() const {
        return _ip < _code->get_num_instructions();
    }

    const Code::Instruction& Stack::Frame::get_next_instruction() const {
        return (*_code)[_ip];
    }

    const Module* Stack::Frame::get_globals() const {
        return _globals;
    }

    Module* Stack::Frame::get_globals() {
        return _globals;
    }

    const Object* Stack::Frame::get_global(const std::string& name) const {
        return _globals->get_property(name);
    }

    Object* Stack::Frame::get_global(const std::string& name) {
        return _globals->get_property(name);
    }

    void Stack::Frame::set_global(const std::string& name, Object* val) {
        _globals->set_property(name, val);
    }

    const std::unordered_map<std::string, Object*>& Stack::Frame::get_locals() const {
        return _locals;
    }

    const Object* Stack::Frame::get_local(const std::string& name) const {
        return _locals.at(name);
    }

    Object* Stack::Frame::get_local(const std::string& name) {
        return _locals.at(name);
    }

    void Stack::Frame::set_local(const std::string& name, Object* val) {
        _locals[name] = val;
    }

    size_t Stack::Frame::num_locals() const {
        return _locals.size();
    }

    const std::deque<Object*> Stack::Frame::get_data_stack() const {
        return _data_stack;
    }

    const Object* Stack::Frame::peek_ds() const {
        CHECK_THROW_LOGIC_ERROR(!_data_stack.empty(), "cannot peek an empty stack");

        return _data_stack.back();
    }

    Object* Stack::Frame::peek_ds() {
        CHECK_THROW_LOGIC_ERROR(!_data_stack.empty(), "cannot peek an empty stack");

        return _data_stack.back();
    }

    Object* Stack::Frame::pop_ds() {
        CHECK_THROW_LOGIC_ERROR(!_data_stack.empty(), "cannot pop an empty stack");

        Object* obj = _data_stack.back();
        _data_stack.pop_back();
        return obj;
    }

    std::vector<Object*> Stack::Frame::pop_n_ds(size_t n) {
        std::vector<Object*> vec;
        for (size_t i = 0; i < n; i++) {
            vec.push_back(pop_ds());
        }

        return vec;
    }

    void Stack::Frame::push_ds(Object* val) {
        _data_stack.push_back(val);
    }

} // namespace emerald
