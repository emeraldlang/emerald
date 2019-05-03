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

#ifndef _EMERALD_RUN_QUEUE_H
#define _EMERALD_RUN_QUEUE_H

#include <memory>
#include <queue>
#include <vector>

#include "emerald/blocking_queue.h"
#include "emerald/process.h"

namespace emerald {

    using RunQueue = BlockingQueue<std::shared_ptr<Process>, std::priority_queue<
        std::shared_ptr<Process>,
        std::vector<std::shared_ptr<Process>>,
        Process::SharedProcessPriorityComparator>
    >;

} // namespace emerald

#endif // _EMERALD_RUN_QUEUE_H
