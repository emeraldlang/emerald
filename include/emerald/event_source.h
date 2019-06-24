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

#ifndef _EMERALD_EVENT_SOURCE_H
#define _EMERALD_EVENT_SOURCE_H

#include <functional>

#include "emerald/object.h"
#include "emerald/process.h"

namespace emerald {

    class EventSource : public HeapObject {
    public:
        using PollingFunction = std::function<Object*()>;

        EventSource(Process* attached_process, PollingFunction polling_function);

        bool done();
        void poll();

        Object* get_exception();
        const Object* get_exception() const;

        Object* get_result();
        const Object* get_result() const;

    private:
        Process* _process;
        PollingFunction _polling_function;

        Object* _exception;
        Object* _result;
    };

} // namespace emerald

#endif // _EMERALD_EVENT_SOURCE_H
