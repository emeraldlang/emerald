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

#include "emerald/heap_managed.h"

namespace emerald {

    HeapManaged::HeapManaged(Heap* heap)
        : _heap(heap), 
        _marked(false) {}

    HeapManaged::~HeapManaged() {}

    bool HeapManaged::is_marked() const {
        return _marked;
    }

    void HeapManaged::mark() {
        _marked = true;
        reach();
    }

    void HeapManaged::unmark() {
        _marked = false;
    }

    void HeapManaged::reach() {}

    Heap* HeapManaged::get_heap() {
        return _heap;
    }
    
    const Heap* HeapManaged::get_heap() const {
        return _heap;
    }

} // namespace emerald
