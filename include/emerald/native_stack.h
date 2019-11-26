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

#ifndef _EMERALD_NATIVE_STACK_H
#define _EMERALD_NATIVE_STACK_H

#include <deque>
#include <string>
#include <unordered_map>
#include <vector>

#include "emerald/heap_root_source.h"

namespace emerald {

    class Object;
    class Module;

    class NativeStack : public HeapRootSource {
    public:
        NativeStack() = default;

        class NativeFrame {
        public:
            NativeStack& get_stack();
            const NativeStack& get_stack() const;

            Object* get_receiver();
            const Object* get_receiver() const;

            const std::vector<Object*>& get_args() const;
            size_t num_args() const;

            Object* get_arg(size_t arg_i);
            const Object* get_arg(size_t arg_i) const;

            const Module* get_globals() const;
            Module* get_globals();

            const Object* get_global(const std::string& name) const;
            Object* get_global(const std::string& name);

            void set_global(const std::string& name, Object* val);

            const std::vector<Object*>& get_locals() const;

            void add_local(Object* obj);

        private:
            NativeStack& _stack;
            Object* _receiver;
            std::vector<Object*> _args;
            Module* _globals;
            std::vector<Object*> _locals;

            friend class NativeStack;

            NativeFrame(NativeStack& stack);
            NativeFrame(NativeStack& stack, Object* receiver, const std::vector<Object*>& args, Module* globals);
        };

        class ScopedNativeFrame {
        public:
            ScopedNativeFrame(NativeFrame& frame);
            ~ScopedNativeFrame();

            const NativeFrame& frame() const;
            NativeFrame& frame();

        private:
            NativeFrame& _frame;
        };

        NativeFrame& peek();

        void pop_frame();
        NativeFrame& push_frame();
        NativeFrame& push_frame(Object* receiver, const std::vector<Object*>& args, Module* globals);

        std::vector<HeapManaged*> get_roots() override;

    private:
        std::deque<NativeFrame> _stack;
    };

} // namespace emerald

#endif // _EMERALD_NATIVE_STACK_H
