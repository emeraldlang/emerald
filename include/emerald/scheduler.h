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

#ifndef _EMERALD_SCHEDULER_H
#define _EMERALD_SCHEDULER_H

#include <atomic>
#include <memory>
#include <queue>
#include <thread>
#include <vector>

#include "emerald/no_copy.h"
#include "emerald/process_map.h"
#include "emerald/run_queue.h"

namespace emerald {

    class Scheduler {
    public:
        Scheduler(
            std::shared_ptr<ProcessMap> process_map, 
            std::shared_ptr<RunQueue> run_queue);
        Scheduler(Scheduler&& other);

        NO_COPY(Scheduler);
        
        bool is_running() const;

        void start();
        void stop();

    private:
        std::shared_ptr<ProcessMap> _process_map;
        std::shared_ptr<RunQueue> _run_queue;
        std::atomic_bool _running;
        std::thread _thread;

        void set_running(bool val);
        void run();
    };

} // namespace emerald

#endif // _EMERALD_SCHEDULER_H
