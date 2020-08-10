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
#include <thread>

#include "emerald/interpreter.h"
#include "emerald/module.h"
#include "emerald/modules/process.h"
#include "emerald/native_variables.h"
#include "emerald/objectutils.h"
#include "emerald/process.h"

namespace emerald {
namespace modules {

    NATIVE_FUNCTION(process_create) {
        EXPECT_ATLEAST_NUM_ARGS(1);

        Process* new_process = ProcessManager::create();
        CloneCache cache;
        new_process->get_heap().add_root_source(&cache);
        Object* callable = frame->get_arg(0)->clone(new_process, cache);
        std::vector<Object*> args;
        for (size_t i = 1; i < frame->num_args(); i++) {
            args.push_back(frame->get_arg(i)->clone(new_process, cache));
        }
        Object* receiver = frame->get_receiver()->clone(new_process, cache);
        new_process->get_heap().remove_root_source(&cache);
        ProcessManager::execute(new_process->get_id(), [=](emerald::Process*) {
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

    NATIVE_FUNCTION(process_join) {
        EXPECT_NUM_ARGS(1);

        CONVERT_ARG_TO(0, Number, pid);

        ProcessManager::join(pid->get_native_value());

        return NONE;
    }

    NATIVE_FUNCTION(process_receive) {
        EXPECT_NUM_ARGS(0);

        return process->get_mailbox().pop_msg();
    }

    NATIVE_FUNCTION(process_send) {
        EXPECT_NUM_ARGS(2);

        CONVERT_ARG_TO(0, Number, pid);

        if (Process* receiver = ProcessManager::get(pid->get_native_value())) {
            CloneCache cache;
            receiver->get_heap().add_root_source(&cache);
            Object* copy = frame->get_arg(1)->clone(receiver, cache);
            receiver->get_heap().remove_root_source(&cache);
            receiver->get_mailbox().push_msg(copy);
            return BOOLEAN(true);
        }

        return BOOLEAN(false);
    }

    NATIVE_FUNCTION(process_sleep) {
        EXPECT_NUM_ARGS(1);

        CONVERT_ARG_TO(0, Number, time);

        std::this_thread::sleep_for(
            std::chrono::duration<double>(time->get_native_value()));

        return NONE;
    }

    NATIVE_FUNCTION(process_state) {
        EXPECT_NUM_ARGS(1);

        CONVERT_ARG_TO(0, Number, pid);

        if (Process* process = ProcessManager::get(pid->get_native_value())) {
            switch (process->get_state()) {
            case Process::State::PENDING:
                return ALLOC_STRING("pending");
            case Process::State::RUNNING:
                return ALLOC_STRING("running");
            case Process::State::COMPLETED:
                return ALLOC_STRING("completed");
            }
        }

        return ALLOC_STRING("unknown");
    }

    MODULE_INITIALIZATION_FUNC(init_process_module) {
        Process* process = module->get_process();

        module->set_property("create", ALLOC_NATIVE_FUNCTION(process_create));
        module->set_property("id", ALLOC_NATIVE_FUNCTION(process_id));
        module->set_property("join", ALLOC_NATIVE_FUNCTION(process_join));
        module->set_property("receive", ALLOC_NATIVE_FUNCTION(process_receive));
        module->set_property("send", ALLOC_NATIVE_FUNCTION(process_send));
        module->set_property("sleep", ALLOC_NATIVE_FUNCTION(process_sleep));
        module->set_property("state", ALLOC_NATIVE_FUNCTION(process_state));

        Local<Object> states = ALLOC_OBJECT();
        states->set_property("pending", ALLOC_STRING("pending"));
        states->set_property("running", ALLOC_STRING("running"));
        states->set_property("completed", ALLOC_STRING("completed"));
        module->set_property("States", states.val());
    }

} // namespace modules
} // namespace emerald
