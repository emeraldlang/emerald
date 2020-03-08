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

#ifndef _EMERALD_NATIVE_VARIABLES_H
#define _EMERALD_NATIVE_VARIABLES_H

namespace emerald {

    template <class T>
    class Local {
    public:
        Local(T* obj);

        T* val() const {
            return _obj;
        }

        T* operator->() const {
            return _obj;
        }

        T& operator*() const {
            return *_obj;
        }

    private:
        T* _obj;
    };

    template <class T>
    Local<T>::Local(T* obj)
        : _obj(obj) {
        _obj
            ->get_process()
            ->get_native_stack()
            .peek()
            .add_local(_obj);
    }

} // namespace emerald

#endif // _EMERALD_NATIVE_VARIABLES_H
