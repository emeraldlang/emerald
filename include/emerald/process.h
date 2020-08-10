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

#include <atomic>
#include <mutex>
#include <thread>
#include <unordered_map>

#include "emerald/heap.h"
#include "emerald/mailbox.h"
#include "emerald/module_registry.h"
#include "emerald/native_objects.h"
#include "emerald/native_stack.h"
#include "emerald/stack.h"

namespace emerald {

    class ProcessManager;

    class Process {
    public:
        using PID = size_t;

        enum class State {
            PENDING,
            RUNNING,
            COMPLETED
        };

        Process(PID id);

        PID get_id() const { return _id; }

        const Heap& get_heap() const { return _heap; }
        Heap& get_heap() { return _heap; }

        const Mailbox& get_mailbox() const { return _mailbox; }
        Mailbox& get_mailbox() { return _mailbox; }

        const ModuleRegistry& get_module_registry() const { return _module_registry; }
        ModuleRegistry& get_module_registry() { return _module_registry; }

        const NativeObjects& get_native_objects() const { return _native_objects; }
        NativeObjects& get_native_objects() { return _native_objects; }

        const NativeStack& get_native_stack() const { return _native_stack; }
        NativeStack& get_native_stack() { return _native_stack; }

        const Stack& get_stack() const { return _stack; }
        Stack& get_stack() { return _stack; }

        State get_state() const { return _state.load(); }
        void set_state(State state) { _state.store(state); }

    private:
        friend class ProcessManager;

        PID _id;
        std::atomic<State> _state;

        Heap _heap;
        Mailbox _mailbox;
        ModuleRegistry _module_registry;
        NativeObjects _native_objects;
        NativeStack _native_stack;
        Stack _stack;
    };

    class ProcessManager {
    public:
        static Process* create();
        static void execute(Process::PID id, std::function<void(Process*)> f);
        static Process* get(Process::PID id);
        static void join(Process::PID id);

    private:
        static Process::PID _curr_id;
        static std::unordered_map<Process::PID, Process> _map;
        static std::unordered_map<Process::PID, std::thread> _threads;
        static std::mutex _mutex;
    };

} // namespace emerald

#endif // _EMERALD_PROCESS_H
