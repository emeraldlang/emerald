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

#ifndef _EMERALD_HEAP_H
#define _EMERALD_HEAP_H

#include <unordered_set>
#include <utility>

#include "emerald/heap_managed.h"
#include "emerald/heap_root_source.h"

namespace emerald {

    class Heap {
    public:
        Heap();
        ~Heap();

        const std::unordered_set<HeapManaged*>& get_managed_set() const;
        size_t get_managed_count() const;
        void add_managed(HeapManaged* managed);

        template <class T, class... Args>
        T* allocate(Args&&... args);

        void add_root_source(HeapRootSource* root_source);
        void remove_root_source(HeapRootSource* root_source);
        
        void collect();

    private:
        std::unordered_set<HeapManaged*> _managed_set;
        std::unordered_set<HeapRootSource*> _root_source_set;

        size_t _threshold;
    };

    template <class T, class... Args>
    T* Heap::allocate(Args&&... args) {
        if (_managed_set.size() >= _threshold) {
            collect();
            _threshold *= 2;
        }

        T* managed = new T(std::forward<Args>(args)...);
        _managed_set.insert(managed);
        return managed;
    }

} // namespace emerald

#endif // _EMERALD_HEAP_H
