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

#ifndef _EMERALD_CONCURRENT_MAP_H
#define _EMERALD_CONCURRENT_MAP_H

#include <functional>
#include <memory>
#include <mutex>
#include <unordered_map>

#include "emerald/no_copy.h"

namespace emerald {

    // TODO(zvp): use lock free implementation
    template <
        class Key, 
        class T,
        class Hash = std::hash<Key>,
        class KeyEqual = std::equal_to<Key>,
        class Allocator = std::allocator<std::pair<const Key, T>>
    >
    class ConcurrentMap {
    public:
        ConcurrentMap() = default;
        ConcurrentMap(ConcurrentMap&& other)
            : _map(std::move(other._map)),
            _mutex(std::move(other._mutex)) {}

        NO_COPY(ConcurrentMap);

        bool empty() const {
            std::lock_guard<std::mutex> lock(_mutex);
            return _map.empty();
        }

        size_t size() const {
            std::lock_guard<std::mutex> lock(_mutex);
            return _map.size();
        }

        T& get(const Key& key) {
            std::lock_guard<std::mutex> lock(_mutex);
            return _map.at(key);
        }

        const T& get(const Key& key) const {
            std::lock_guard<std::mutex> lock(_mutex);
            return _map.at(key);
        }

        void put(const Key& key, const T& val) {
            std::lock_guard<std::mutex> lock(_mutex);
            _map[key] = val;
        }

        bool remove(const Key& key) {
            size_t elems_removed;
            {
                std::lock_guard<std::mutex> lock(_mutex);
                elems_removed = _map.erase(key);
            }
            return elems_removed > 0;
        }

        T& operator[](const Key& key) {
            return get(key);
        }

        const T& operator[](const Key& key) const {
            return get(key);
        }

        ConcurrentMap& operator=(ConcurrentMap&& other) {
            if (this == &other) return *this;

            _map = std::move(other._map);
            _mutex = std::move(other._mutex);

            return *this;
        }

    private:
        std::unordered_map<Key, T, Hash, KeyEqual, Allocator> _map;
        std::mutex _mutex;
    };

} // namespace emerald

#endif // _EMERALD_CONCURRENT_MAP_H
