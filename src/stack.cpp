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

    void Stack::push_frame(std::shared_ptr<const Code> code) {
        _stack.emplace_back(code);
    }

    std::vector<HeapManaged*> Stack::get_roots() const {
        std::vector<HeapManaged*> roots;
        for (const Frame& frame : _stack) {
            for (Object* local : frame.get_locals()) {
                if (HeapObject* heap_obj = dynamic_cast<HeapObject*>(local)) {
                    roots.push_back(heap_obj);
                }
            }
        }

        return roots;
    }

    Stack::Frame::Frame(std::shared_ptr<const Code> code)
        : _code(code), 
        _ip(0) {}

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

    const std::vector<Object*>& Stack::Frame::get_locals() const {
        return _locals;
    }

    const Object* Stack::Frame::get_local(size_t i) const {
        return _locals.at(i);
    }

    std::vector<Object*>& Stack::Frame::get_locals() {
        return _locals;
    }

    Object* Stack::Frame::get_local(size_t i) {
        return _locals.at(i);
    }

    void Stack::Frame::allocate_locals(size_t n) {
        _locals.resize(n);
    }

    void Stack::Frame::set_local(size_t i, Object* obj) {
        _locals.at(i) = obj;
    }

    size_t Stack::Frame::num_locals() const {
        return _locals.size();
    }

} // namespace emerald
