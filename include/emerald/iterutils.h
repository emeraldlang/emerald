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

#ifndef _EMERALD_ITERUTILS_H
#define _EMERALD_ITERUTILS_H

#include <utility>

namespace emerald {
namespace iterutils {

    template <class T>
    class ReverseContainer {
    public:
        ReverseContainer(const T& container)
            : _container(container) {}

        auto begin() {
            return _container.rbegin();
        }

        auto end() {
            return _container.rend();
        }

    private:
        T _container;
    };

    template <class T>
    ReverseContainer<T> reverse(const T& container) {
        return ReverseContainer(container);
    }

} // namespace iterutils
} // namespace emerald

#endif // _EMERALD_ITERUTILS_H
