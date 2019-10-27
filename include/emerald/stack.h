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

#ifndef _EMERALD_STACK_H
#define _EMERALD_STACK_H

#include <cstdint>
#include <deque>
#include <memory>
#include <stack>
#include <vector>

#include "emerald/code.h"
#include "emerald/heap_managed.h"
#include "emerald/heap_root_source.h"

namespace emerald {

    class Object;
    class Module;

    class Stack : public HeapRootSource {
    public:
        static const uint16_t DEFAULT_MAX_SIZE = 8192;

        Stack(uint16_t max_size = DEFAULT_MAX_SIZE);

        class Frame {
        public:
            Frame(Object* receiver, std::shared_ptr<const Code> code, Module* globals);

            Object* get_receiver() const;
            std::shared_ptr<const Code> get_code() const;

            size_t get_instruction_pointer() const;

            void increment_instruction_pointer();
            void set_instruction_pointer(size_t ip);

            bool has_instructions_left() const;

            const Code::Instruction& get_next_instruction() const;

            const Module* get_globals() const;
            Module* get_globals();

            const Object* get_global(const std::string& name) const;
            Object* get_global(const std::string& name);

            void set_global(const std::string& name, Object* val);

            const std::unordered_map<std::string, Object*>& get_locals() const;

            const Object* get_local(const std::string& name) const;
            Object* get_local(const std::string& name);

            void set_local(const std::string& name, Object* val);

            size_t num_locals() const;

            const std::deque<Object*> get_data_stack() const;

            const Object* peek_ds() const;
            Object* peek_ds();

            Object* pop_ds();
            std::vector<Object*> pop_n_ds(size_t n);
            void push_ds(Object* val);

            void push_catch_ip(size_t ip);
            void pop_catch_ip();
            bool has_catch_ip();
            size_t get_catch_ip();

        private:
            Object* _receiver;

            std::shared_ptr<const Code> _code;
            size_t _ip;

            Module* _globals;
            std::unordered_map<std::string, Object*> _locals;
            std::deque<Object*> _data_stack;
            std::stack<size_t> _catch_stack;
        };

        uint16_t max_size() const;
        size_t size() const;

        bool empty() const;

        const Frame& peek() const;
        Frame& peek();

        bool pop_frame();
        void push_frame(Object* receiver, std::shared_ptr<const Code> code, Module* globals);

        const Module* peek_globals() const;
        Module* peek_globals();

        std::vector<HeapManaged*> get_roots() override;

    private:
        uint16_t _max_size;

        std::deque<Frame> _stack;
    };

} // namespace emerald

#endif // _EMERALD_STACK_H
