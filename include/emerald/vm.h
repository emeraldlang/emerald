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

#ifndef _EMERALD_VM_H
#define _EMERALD_VM_H

#include <cstdint>
#include <memory>
#include <thread>
#include <vector>

#include "emerald/code.h"
#include "emerald/process.h"
#include "emerald/process_map.h"
#include "emerald/run_queue.h"
#include "emerald/scheduler.h"

namespace emerald {

    class VM {
    public:
        VM(size_t num_schedulers = std::thread::hardware_concurrency());

        void start();
        void stop();

        void scale(size_t num_schedulers);

        Process::PID create_process(std::shared_ptr<Code> code);
        bool get_process_state(Process::PID id, Process::State& state) const;

    private:
        Process::PID _current_process_id;
        bool _running;

        std::shared_ptr<ProcessMap> _process_map;
        std::shared_ptr<RunQueue> _run_queue;
        std::vector<Scheduler> _schedulers;
    };

} // namespace emerald

#endif // _EMERALD_VM_H 