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

#include <condition_variable>
#include <mutex>
#include <queue>
#include <unordered_map>

#include "emerald/heap.h"
#include "emerald/module_registry.h"
#include "emerald/native_objects.h"
#include "emerald/stack.h"

namespace emerald {

    class Processes;

    class Process {
    public:
        using PID = size_t;

        Process(PID id);

        PID get_id() const { return _id; }

        const Heap& get_heap() const { return _heap; }
        Heap& get_heap() { return _heap; }

        const ModuleRegistry& get_module_registry() const { return _module_registry; }
        ModuleRegistry& get_module_registry() { return _module_registry; }

        const NativeObjects& get_native_objects() const { return _native_objects; }
        NativeObjects& get_native_objects() { return _native_objects; }

        const Stack& get_stack() const { return _stack; }
        Stack& get_stack() { return _stack; }

        void push_msg(Object* message);
        Object* pop_msg();

    private:
        PID _id;

        Heap _heap;
        ModuleRegistry _module_registry;
        NativeObjects _native_objects;
        Stack _stack;

        std::queue<Object*> _mailbox;
        std::mutex _mutex;
        std::condition_variable _cv;
    };

    class Processes {
    public:
        static Process* create_process();
        static Process* get_process(Process::PID id);

    private:
        static Process::PID _curr_id;
        static std::unordered_map<Process::PID, Process> _map;
    };

} // namespace emerald

#endif // _EMERALD_PROCESS_H
