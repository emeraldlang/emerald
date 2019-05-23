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
#include <vector>

#include "emerald/code.h"
#include "emerald/heap_managed.h"
#include "emerald/heap_root_source.h"
#include "emerald/object.h"

namespace emerald {

    class Stack : public HeapRootSource {
    public:
        static const uint16_t DEFAULT_MAX_SIZE = 8192;

        Stack(uint16_t max_size = DEFAULT_MAX_SIZE);

        class Frame {
        public:
            Frame(std::shared_ptr<const Code> code);

            std::shared_ptr<const Code> get_code() const;

            size_t get_instruction_pointer() const;

            void increment_instruction_pointer();
            void set_instruction_pointer(size_t ip);

            bool has_instructions_left() const;

            const Code::Instruction& get_next_instruction() const;

            const std::unordered_map<std::string, Object*>& get_locals() const;

            const Object* get_local(const std::string& name) const;
            Object* get_local(const std::string& name);

            void set_local(const std::string& name, Object* obj);

            size_t num_locals() const;

        private:
            std::shared_ptr<const Code> _code;
            size_t _ip;

            std::unordered_map<std::string, Object*> _locals;
        };

        uint16_t max_size() const;
        size_t size() const;

        bool empty() const;

        const Frame& peek() const;
        Frame& peek();

        bool pop_frame();
        void push_frame(std::shared_ptr<const Code> code);

        std::vector<HeapManaged*> get_roots() const override;

    private:
        uint16_t _max_size;

        std::deque<Frame> _stack;
    };

} // namespace emerald

#endif // _EMERALD_STACK_H
