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

#ifndef _EMERALD_PROCESS_H
#define _EMERALD_PROCESS_H

#include <cstdint>
#include <memory>
#include <mutex>
#include <stack>
#include <queue>
#include <iostream>

#include "emerald/code.h"
#include "emerald/concurrent_map.h"
#include "emerald/data_stack.h"
#include "emerald/heap.h"
#include "emerald/native_prototypes.h"
#include "emerald/no_copy.h"
#include "emerald/object.h"
#include "emerald/stack.h"

namespace emerald {

    class Process {
    public:
        using PID = int32_t;

        static const PID INVALID_PID = -1;

        Process(
            PID id, 
            std::shared_ptr<Code> code, 
            std::shared_ptr<Process> = nullptr, 
            uint8_t priority = 0, 
            uint16_t max_stack_size = Stack::DEFAULT_MAX_SIZE);
        NO_COPY(Process);
        
        PID get_id() const;

        std::shared_ptr<const Process> get_parent_process() const;
        std::shared_ptr<Process> get_parent_process();

        uint8_t get_priority() const;

        enum State {
            start,
            running,
            suspended,
            terminated
        };

        State get_state() const;

        void set_suspended();
        void set_running();

        bool is_running() const;
        bool is_suspended() const;
        bool is_terminated() const;

        void receive(Object* message);

        void execute();

        struct ProcessPriorityComparator {
            bool operator()(const Process& lhs, const Process& rhs) const;
        };

        struct SharedProcessPriorityComparator {
            bool operator()(std::shared_ptr<const Process> lhs, std::shared_ptr<const Process> rhs) const;
        };

    private:
        PID _id;
        std::shared_ptr<Process> _parent_process;
        uint8_t _priority;

        DataStack _data_stack;

        Stack _stack;
        Heap _heap;
        NativePrototypes _native_prototypes;

        State _state;

        std::mutex _messages_mutex;
        std::queue<Object*> _messages;

        void dispatch();

        void execute_mm(
            const std::string& magic_method,
            const std::vector<Object*>& args,
            std::function<Object*(Object*)> on_missing = nullptr);
        void execute_mm(
            const std::string& magic_method,
            size_t nargs,
            std::function<Object*(Object*)> on_missing = nullptr);

        void execute_mm1(
            const std::string& magic_method,
            std::function<Object*(Object*)> on_missing = nullptr) {
            execute_mm(magic_method, 1, on_missing);
        }

        void execute_mm2(
            const std::string& magic_method,
            std::function<Object*(Object*)> on_missing = nullptr) {
            execute_mm(magic_method, 2, on_missing);
        }

        Object* new_obj(bool explicit_parent, size_t num_props);
        void call_obj(Object* obj, const std::vector<Object*>& args);

        void pop_n_from_stack(std::vector<Object*>& vec, size_t n);
        std::vector<Object*> pop_n_from_stack(size_t n);

        Array* allocate_array() {
            return _heap.allocate<Array>(_native_prototypes.get_array_prototype());
        }
        
        Number* allocate_number(double value) {
            return _heap.allocate<Number>(_native_prototypes.get_number_prototype(), value);
        }
        
        String* allocate_string(const std::string& value) {
            return _heap.allocate<String>(_native_prototypes.get_string_prototype(), value);
        }
    };

}

#endif // _EMERALD_PROCESS_H
