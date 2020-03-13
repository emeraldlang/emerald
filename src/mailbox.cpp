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

#include "emerald/mailbox.h"
#include "emerald/object.h"

namespace emerald {

    void Mailbox::push_msg(Object* message) {
        {
            std::lock_guard<std::mutex> lock(_mutex);
            _mailbox.push_back(message);
        }

        _cv.notify_one();
    }

    Object* Mailbox::pop_msg() {
        std::unique_lock<std::mutex> lock(_mutex);
        while (_mailbox.empty()) _cv.wait(lock);

        Object* message = _mailbox.front();
        _mailbox.pop_front();

        return message;
    }

    std::vector<HeapManaged*> Mailbox::get_roots() {
        std::vector<HeapManaged*> roots;
        for (Object* msg : _mailbox) {
            roots.push_back(msg);
        }

        return roots;
    }

} // namespace emerald
