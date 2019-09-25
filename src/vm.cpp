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

#include "emerald/check.h"
#include "emerald/code_cache.h"
#include "emerald/vm.h"

namespace emerald {

    VM::VM(size_t num_schedulers)
        : _current_process_id(0),
        _running(false),
        _process_map(std::make_shared<ProcessMap>()),
        _run_queue(std::make_shared<RunQueue>()) {
        for (size_t i = 0; i < num_schedulers; i++) {
            _schedulers.emplace_back(_process_map, _run_queue);
        }
    }

    void VM::start() {
       CHECK_THROW_LOGIC_ERROR(!_running, "vm is already started");

        for (Scheduler& scheduler : _schedulers) {
            scheduler.start();
        }

        _running = true;
    }

    void VM::stop() {
        CHECK_THROW_LOGIC_ERROR(_running, "vm is already stopped");

        for (Scheduler& scheduler : _schedulers) {
            scheduler.stop();
        }

        _running = false;
    }

    size_t VM::num_schedulers() const {
        return _schedulers.size();
    }

    void VM::scale(size_t num_schedulers) {
        if (num_schedulers < _schedulers.size()) {
            size_t num_to_remove = _schedulers.size() - num_schedulers;
            for (size_t i = 0; i < num_to_remove; i++) {
                _schedulers.back().stop();
                _schedulers.pop_back();
            }
        } else if (num_schedulers > _schedulers.size()) {
            size_t num_to_create = num_schedulers - _schedulers.size();
            for (size_t i = 0; i < num_to_create; i++) {
                _schedulers.emplace_back(_process_map, _run_queue);
                _schedulers.back().start();
            }
        } else {
            // nop
        }
    }

    Process::PID VM::create_process(const std::string& entry_module_name) {
        CHECK_THROW_LOGIC_ERROR(_running, "vm is not running");

        std::shared_ptr<Code> code = CodeCache::get_or_load_code(entry_module_name);

        Process::PID id = _current_process_id++;
        std::shared_ptr<Process> process = std::make_shared<Process>(id, code);

        _process_map->put(id, process);
        _run_queue->push(process);

        return id;
    }

} // namespace emerald
