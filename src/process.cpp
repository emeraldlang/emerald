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

#include "emerald/process.h"
#include "emerald/modules/core.h"

namespace emerald {

    Process::Process(PID id)
        : _id(id),
        _state(State::PENDING),
        _native_objects(this) {
        _heap.add_root_source(&_mailbox);
        _heap.add_root_source(&_module_registry);
        _heap.add_root_source(&_native_objects);
        _heap.add_root_source(&_native_stack);
        _heap.add_root_source(&_stack);
    }

    Process::PID ProcessManager::_curr_id = 0;

    std::unordered_map<Process::PID, Process> ProcessManager::_map;
    std::unordered_map<Process::PID, std::thread> ProcessManager::_threads;
    std::mutex ProcessManager::_mutex;

    Process* ProcessManager::create() {
        std::lock_guard<std::mutex> lock(_mutex);
        Process::PID pid = _curr_id++;
        _map.emplace(pid, pid);
        return &_map.at(pid);
    }

    void ProcessManager::execute(Process::PID id, std::function<void(Process*)> f) {
        std::lock_guard<std::mutex> lock(_mutex);
        if (_threads.find(id) != _threads.end()) {
            return;
        }

        Process* process = &_map.at(id);
        _threads.emplace(id, [=]() {
            process->_state.store(Process::State::RUNNING);
            f(process);
            process->_state.store(Process::State::COMPLETED);
        });
    }

    Process* ProcessManager::get(Process::PID id) {
        std::lock_guard<std::mutex> lock(_mutex);
        if (_map.find(id) != _map.end()) {
            return &_map.at(id);
        }
        return nullptr;
    }

    void ProcessManager::join(Process::PID id) {
        _mutex.lock();
        if (_threads.find(id) == _threads.end()) return;
        std::thread& t = _threads.at(id);
        _mutex.unlock();
        // join after unlock to prevent deadlock
        if (t.joinable()) {
            t.join();
        }
    }

} // namespace emerald
