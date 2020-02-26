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

#include "emerald/interpreter.h"
#include "emerald/module.h"
#include "emerald/modules/process.h"
#include "emerald/objectutils.h"
#include "emerald/process.h"
#include "emerald/thread_pool.h"

namespace emerald {
namespace modules {

    NATIVE_FUNCTION(process_create) {
        EXPECT_ATLEAST_NUM_ARGS(1);

        Process* new_process = Processes::create_process();
        CloneCache cache;
        new_process->get_heap().add_root_source(&cache);
        Object* callable = frame->get_arg(0)->clone(new_process, cache);
        std::vector<Object*> args;
        for (size_t i = 1; i < frame->num_args(); i++) {
            args.push_back(frame->get_arg(i)->clone(new_process, cache));
        }
        Object* receiver = frame->get_receiver()->clone(new_process, cache);
        new_process->get_heap().remove_root_source(&cache);
        ThreadPool::queue([=]() {
            Interpreter::call_obj<Object>(
                callable,
                receiver,
                args,
                new_process);
        });

        return ALLOC_NUMBER(new_process->get_id());
    }

    NATIVE_FUNCTION(process_id) {
        return ALLOC_NUMBER(process->get_id());
    }

    NATIVE_FUNCTION(process_receive) {
        EXPECT_NUM_ARGS(0);

        return process->pop_msg();
    }

    NATIVE_FUNCTION(process_send) {
        EXPECT_NUM_ARGS(2);

        TRY_CONVERT_ARG_TO(0, Number, pid);

        if (Process* receiver = Processes::get_process(pid->get_native_value())) {
            CloneCache cache;
            receiver->get_heap().add_root_source(&cache);
            Object* copy = frame->get_arg(1)->clone(receiver, cache);
            receiver->get_heap().remove_root_source(&cache);
            receiver->push_msg(copy);
            return BOOLEAN(true);
        }

        return BOOLEAN(false);
    }

    MODULE_INITIALIZATION_FUNC(init_process_module) {
        Process* process = module->get_process();

        module->set_property("create", ALLOC_NATIVE_FUNCTION(process_create));
        module->set_property("id", ALLOC_NATIVE_FUNCTION(process_id));
        module->set_property("receive", ALLOC_NATIVE_FUNCTION(process_receive));
        module->set_property("send", ALLOC_NATIVE_FUNCTION(process_send));
    }

} // namespace modules
} // namespace emerald
