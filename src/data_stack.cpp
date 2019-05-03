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

#include "emerald/data_stack.h"

namespace emerald {

    void DataStack::push(Object* obj) {
        _stack.push_back(obj);
    }

    Object* DataStack::pop() {
        Object* obj = _stack.back();
        _stack.pop_back();
        return obj;
    }

    Object* DataStack::peek() {
        return _stack.back();
    }
    
    const Object* DataStack::peek() const {
        return _stack.back();
    }

    bool DataStack::empty() const {
        return _stack.empty();
    }
    
    size_t DataStack::size() const {
        return _stack.size();
    }

    std::vector<HeapManaged*> DataStack::get_roots() const {
        std::vector<HeapManaged*> roots;
        for (Object* obj : _stack) {
            if (HeapObject* heap_obj = dynamic_cast<HeapObject*>(obj)) {
                roots.push_back(heap_obj);
            }
        }

        return roots;
    }

} // namespace emerald
