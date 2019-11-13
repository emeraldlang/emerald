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
        _native_objects(this) {
        _heap.add_root_source(&_module_registry);
        _heap.add_root_source(&_native_objects);
        _heap.add_root_source(&_stack);
    }

    void Process::push_msg(Object* message) {
        {
            std::lock_guard<std::mutex> lock(_mutex);
            _mailbox.push(message);
        }

        _cv.notify_one();
    }

    Object* Process::pop_msg() {
        std::unique_lock<std::mutex> lock(_mutex);
        while (_mailbox.empty()) _cv.wait(lock);

        Object* message = _mailbox.front();
        _mailbox.pop();

        return message;
    }

    Process* Processes::create_process() {
        Process::PID pid = _curr_id++;
        _map.emplace(pid, pid);
        return &_map.at(pid);
    }

    Process* Processes::get_process(Process::PID pid) {
        return &_map.at(pid);
    }

    Process::PID Processes::_curr_id = 0;

    std::unordered_map<Process::PID, Process> Processes::_map;

} // namespace emerald
