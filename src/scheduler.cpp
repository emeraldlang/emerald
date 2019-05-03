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

#include <chrono>
#include <iostream>

#include "emerald/scheduler.h"
#include "emerald/check.h"

#define QUANTUM 2000

namespace emerald {

    Scheduler::Scheduler(
        std::shared_ptr<ProcessMap> process_map, 
        std::shared_ptr<RunQueue> run_queue)
         : _process_map(process_map), 
         _run_queue(run_queue),
         _running(false) {}

    Scheduler::Scheduler(Scheduler&& other)
        : _process_map(std::move(other._process_map)), 
        _run_queue(std::move(other._run_queue)),
        _running(other._running.load()),
        _thread(std::move(other._thread)) {}

    bool Scheduler::is_running() const {
        return _running.load(std::memory_order_acquire);
    }

    void Scheduler::start() {
        CHECK_THROW_LOGIC_ERROR(!is_running(), "scheduler is already running");

        set_running(true);
        _thread = std::thread([this] { this->run(); });
    }

    void Scheduler::stop() {
        CHECK_THROW_LOGIC_ERROR(is_running(), "scheduler is not running");

        set_running(false);
        _thread.join();
    }

    void Scheduler::set_running(bool val) {
        _running.store(val, std::memory_order_release);
    }

    void Scheduler::run() {
        using namespace std::chrono_literals;

        while (is_running()) {
            std::chrono::time_point<std::chrono::system_clock> timeout_time = 
                std::chrono::system_clock::now() + 10ms;
            std::shared_ptr<Process> process;
            if (_run_queue->pop(process, timeout_time)) {
                DCHECK(process->is_suspended(), "expected process to be suspended");

                process->set_running();

                for (size_t i = 0; i < QUANTUM; i++) {
                    if (process->is_terminated() || process->is_suspended()) break;
                    process->execute();
                }

                if (!process->is_terminated()) {
                    process->set_suspended();
                    _run_queue->push(process);
                }
            }
        }
    }

} // namespace emerald
