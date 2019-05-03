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

#ifndef _EMERALD_HEAP_MANAGED_H
#define _EMERALD_HEAP_MANAGED_H

namespace emerald {

    class Heap;

    class HeapManaged {
    public:
        HeapManaged(Heap* heap);
        virtual ~HeapManaged();

        bool is_marked() const;

        void mark();
        void unmark();

        Heap* get_heap();
        const Heap* get_heap() const;

    protected:
        virtual void reach();

    private:
        Heap* _heap;
        bool _marked;
    };

} // namespace emerald

#endif // _EMERALD_HEAP_MANAGED_H
