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

#ifndef _EMERALD_NATIVE_OBJECTS_H
#define _EMERALD_NATIVE_OBJECTS_H

#include "emerald/object.h"

namespace emerald {

    class ExecutionContext;

    class NativeObjects {
    public:
        NativeObjects(ExecutionContext* context);

        const Object* get_object_prototype() const;
        Object* get_object_prototype();

        const Array* get_array_prototype() const;
        Array* get_array_prototype();

        const Number* get_number_prototype() const;
        Number* get_number_prototype();

        const String* get_string_prototype() const;
        String* get_string_prototype();

        const Boolean* get_boolean_prototype() const;
        Boolean* get_boolean_prototype();

        const Boolean* get_boolean(bool val) const;
        Boolean* get_boolean(bool val);

        const Null* get_null() const;
        Null* get_null();

    private:
        Object* _object;

        Array* _array;
        Number* _number;
        String* _string;

        Boolean* _boolean;
        Boolean* _true;
        Boolean* _false;

        Null* _null;

        void initialize_object(ExecutionContext* context);
        void initialize_array(ExecutionContext* context);
        void initialize_number(ExecutionContext* context);
        void initialize_string(ExecutionContext* context);
        void initialize_booleans(ExecutionContext* context);
    };

} // namespace emerald

#endif // _EMERALD_NATIVE_OBJECTS_H
