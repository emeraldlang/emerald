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

#include "emerald/heap.h"

namespace emerald {

    Heap::Heap()
        : _threshold(512) {}

    Heap::~Heap() {
        for (HeapManaged* managed : _managed_set) {
            delete managed;
        }
    }

    const std::unordered_set<HeapManaged*>& Heap::get_managed_set() const {
        std::lock_guard<std::mutex> lock(_mutex);
        return _managed_set;
    }

    size_t Heap::get_managed_count() const {
        std::lock_guard<std::mutex> lock(_mutex);
        return _managed_set.size();
    }

    void Heap::add_managed(HeapManaged* managed) {
        std::lock_guard<std::mutex> lock(_mutex);
        _managed_set.insert(managed);
    }

    void Heap::add_root_source(HeapRootSource* root_source) {
        std::lock_guard<std::mutex> lock(_mutex);
        _root_source_set.insert(root_source);
    }

    void Heap::remove_root_source(HeapRootSource* root_source) {
        std::lock_guard<std::mutex> lock(_mutex);
        _root_source_set.erase(root_source);
    }

    void Heap::collect() {
        std::lock_guard<std::mutex> lock(_mutex);
        collect_nolock();
    }

    size_t Heap::threshold() const {
        std::lock_guard<std::mutex> lock(_mutex);
        return _threshold;
    }

    void Heap::set_threshold(size_t threshold) {
        std::lock_guard<std::mutex> lock(_mutex);
        _threshold = threshold;
    }

    void Heap::collect_nolock() {
        for (HeapRootSource* root_source : _root_source_set) {
            for (HeapManaged* managed : root_source->get_roots()) {
                managed->mark();
            }
        }

        for (std::unordered_set<HeapManaged*>::iterator it = _managed_set.begin();
            it != _managed_set.end();) {
            HeapManaged* managed = *it;
            if (!managed->is_marked()) {
                it = _managed_set.erase(it);
                delete managed;
            } else {
                managed->unmark();
                it++;
            }
        }
    }

} // namespace emerald
