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

#include <utility>

#include "emerald/check.h"
#include "emerald/native_stack.h"
#include "emerald/module.h"
#include "emerald/object.h"

namespace emerald {

    NativeStack::NativeFrame& NativeStack::peek() {
        CHECK_THROW_LOGIC_ERROR(!_stack.empty(), "cannot peek an empty stack");

        return _stack.back();
    }

    void NativeStack::pop_frame() {
        if (_stack.empty()) return;
        _stack.pop_back();
    }

    NativeStack::NativeFrame& NativeStack::push_frame() {
        NativeFrame frame(*this);
        _stack.push_back(std::move(frame));
        return _stack.back();
    }

    NativeStack::NativeFrame& NativeStack::push_frame(Object* receiver, const std::vector<Object*>& args, Module* globals) {
        NativeFrame frame(*this, receiver, args, globals);
        _stack.push_back(std::move(frame));
        return _stack.back();
    }

    std::vector<HeapManaged*> NativeStack::get_roots() {
        std::vector<HeapManaged*> roots;
        for (NativeFrame& frame : _stack) {
            roots.push_back(frame.get_receiver());
            roots.push_back(frame.get_globals());

            for (Object* local : frame.get_locals()) {
                roots.push_back(local);
            }
        }

        return roots;
    }

    NativeStack::NativeFrame::NativeFrame(NativeStack& stack)
        : _stack(stack) {}

    NativeStack::NativeFrame::NativeFrame(NativeStack& stack, Object* receiver, const std::vector<Object*>& args, Module* globals)
        : _stack(stack), 
        _receiver(receiver),
        _args(args),
        _globals(globals) {}

    NativeStack& NativeStack::NativeFrame::get_stack() {
        return _stack;
    }

    const NativeStack& NativeStack::NativeFrame::get_stack() const {
        return _stack;
    }

    Object* NativeStack::NativeFrame::get_receiver() {
        return _receiver;
    }

    const Object* NativeStack::NativeFrame::get_receiver() const {
        return _receiver;
    }

    const std::vector<Object*>& NativeStack::NativeFrame::get_args() const {
        return _args;
    }

    size_t NativeStack::NativeFrame::num_args() const {
        return _args.size();
    }

    Object* NativeStack::NativeFrame::get_arg(size_t i) {
        return _args.at(i);
    }

    const Object* NativeStack::NativeFrame::get_arg(size_t i) const {
        return _args.at(i);
    }

    const Module* NativeStack::NativeFrame::get_globals() const {
        return _globals;
    }

    Module* NativeStack::NativeFrame::get_globals() {
        return _globals;
    }

    const Object* NativeStack::NativeFrame::get_global(const std::string& name) const {
        if (_globals) return _globals->get_property(name);
        return nullptr;
    }

    Object* NativeStack::NativeFrame::get_global(const std::string& name) {
        if (_globals) return _globals->get_property(name);
        return nullptr;
    }

    void NativeStack::NativeFrame::set_global(const std::string& name, Object* val) {
        if (_globals) _globals->set_property(name, val);
    }

    const std::vector<Object*>& NativeStack::NativeFrame::get_locals() const {
        return _locals;
    }

    void NativeStack::NativeFrame::add_local(Object* obj) {
        _locals.push_back(obj);
    }

    NativeStack::ScopedNativeFrame::ScopedNativeFrame(NativeFrame& frame)
        : _frame(frame) {}

    NativeStack::ScopedNativeFrame::~ScopedNativeFrame() {
        _frame.get_stack().pop_frame();
    }

    const NativeStack::NativeFrame& NativeStack::ScopedNativeFrame::frame() const {
        return _frame;
    }

    NativeStack::NativeFrame& NativeStack::ScopedNativeFrame::frame() {
        return _frame;
    }

} // namespace emerald
