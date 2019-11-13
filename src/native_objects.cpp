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

#include "emerald/native_objects.h"
#include "emerald/magic_methods.h"
#include "emerald/natives/array.h"
#include "emerald/natives/boolean.h"
#include "emerald/natives/exception.h"
#include "emerald/natives/number.h"
#include "emerald/natives/object.h"
#include "emerald/natives/string.h"
#include "emerald/object.h"
#include "emerald/objectutils.h"
#include "emerald/process.h"

namespace emerald {

    NativeObjects::NativeObjects(Process* process) {
        initialize_object(process);
        initialize_array(process);
        initialize_booleans(process);
        initialize_exception(process);
        initialize_number(process);
        initialize_string(process);

        _null = process->get_heap().allocate<Null>(process);
    }

    const Object* NativeObjects::get_object_prototype() const {
        return _object;
    }

    Object* NativeObjects::get_object_prototype() {
        return _object;
    }

    const Array* NativeObjects::get_array_prototype() const {
        return _array;
    }

    Array* NativeObjects::get_array_prototype() {
        return _array;
    }

    const ArrayIterator* NativeObjects::get_array_iterator_prototype() const {
        return _array_iterator;
    }

    ArrayIterator* NativeObjects::get_array_iterator_prototype() {
        return _array_iterator;
    }

    const Exception* NativeObjects::get_exception_prototype() const {
        return _exception;
    }

    Exception* NativeObjects::get_exception_prototype() {
        return _exception;
    }

    const Number* NativeObjects::get_number_prototype() const {
        return _number;
    }

    Number* NativeObjects::get_number_prototype() {
        return _number;
    }

    const String* NativeObjects::get_string_prototype() const {
        return _string;
    }

    String* NativeObjects::get_string_prototype() {
        return _string;
    }

    const Boolean* NativeObjects::get_boolean_prototype() const {
        return _boolean;
    }

    Boolean* NativeObjects::get_boolean_prototype() {
        return _boolean;
    }

    const Boolean* NativeObjects::get_boolean(bool val) const {
        return (val) ? _true : _false;
    }

    Boolean* NativeObjects::get_boolean(bool val) {
        return (val) ? _true : _false;
    }

    const Null* NativeObjects::get_null() const {
        return _null;
    }

    Null* NativeObjects::get_null() {
        return _null;
    }

    std::vector<HeapManaged*> NativeObjects::get_roots() {
        return std::vector<HeapManaged*>({
            _object,
            _array,
            _array_iterator,
            _number,
            _string,
            _boolean,
            _true,
            _false,
            _null
        });
    }

    void NativeObjects::initialize_object(Process* process) {
        _object = process->get_heap().allocate<Object>(process);

        _object->set_property(magic_methods::eq, ALLOC_NATIVE_FUNCTION_NO_MOD(natives::object_eq));
        _object->set_property(magic_methods::neq, ALLOC_NATIVE_FUNCTION_NO_MOD(natives::object_neq));

        _object->set_property(magic_methods::str, ALLOC_NATIVE_FUNCTION_NO_MOD(natives::object_str));
        _object->set_property(magic_methods::boolean, ALLOC_NATIVE_FUNCTION_NO_MOD(natives::object_boolean));

        _object->set_property(magic_methods::clone, ALLOC_NATIVE_FUNCTION_NO_MOD(natives::object_clone));
        _object->set_property(magic_methods::init, ALLOC_NATIVE_FUNCTION_NO_MOD(natives::object_init));

        _object->set_property("keys", ALLOC_NATIVE_FUNCTION_NO_MOD(natives::object_keys));
        _object->set_property("get_prop", ALLOC_NATIVE_FUNCTION_NO_MOD(natives::object_get_prop));
        _object->set_property("set_prop", ALLOC_NATIVE_FUNCTION_NO_MOD(natives::object_set_prop));
    }

    void NativeObjects::initialize_array(Process* process) {
        _array = process->get_heap().allocate<Array>(process, _object);

        _array->set_property(magic_methods::eq, ALLOC_NATIVE_FUNCTION_NO_MOD(natives::array_eq));
        _array->set_property(magic_methods::neq, ALLOC_NATIVE_FUNCTION_NO_MOD(natives::array_neq));

        _array->set_property(magic_methods::iter, ALLOC_NATIVE_FUNCTION_NO_MOD(natives::array_iter));

        _array->set_property(magic_methods::clone, ALLOC_NATIVE_FUNCTION_NO_MOD(natives::array_clone));
        _array->set_property(magic_methods::init, ALLOC_NATIVE_FUNCTION_NO_MOD(natives::array_init));

        _array->set_property("at", ALLOC_NATIVE_FUNCTION_NO_MOD(natives::array_at));
        _array->set_property("front", ALLOC_NATIVE_FUNCTION_NO_MOD(natives::array_front));
        _array->set_property("back", ALLOC_NATIVE_FUNCTION_NO_MOD(natives::array_back));
        _array->set_property("empty", ALLOC_NATIVE_FUNCTION_NO_MOD(natives::array_empty));
        _array->set_property("size", ALLOC_NATIVE_FUNCTION_NO_MOD(natives::array_size));
        _array->set_property("clear", ALLOC_NATIVE_FUNCTION_NO_MOD(natives::array_clear));
        _array->set_property("push", ALLOC_NATIVE_FUNCTION_NO_MOD(natives::array_push));
        _array->set_property("pop", ALLOC_NATIVE_FUNCTION_NO_MOD(natives::array_pop));
        _array->set_property("join", ALLOC_NATIVE_FUNCTION_NO_MOD(natives::array_join));

        _array_iterator = process->get_heap().allocate<ArrayIterator>(process, _object);

        _array_iterator->set_property(magic_methods::cur, ALLOC_NATIVE_FUNCTION_NO_MOD(natives::array_iterator_cur));
        _array_iterator->set_property(magic_methods::done, ALLOC_NATIVE_FUNCTION_NO_MOD(natives::array_iterator_done));
        _array_iterator->set_property(magic_methods::next, ALLOC_NATIVE_FUNCTION_NO_MOD(natives::array_iterator_next));

        _array_iterator->set_property(magic_methods::clone, ALLOC_NATIVE_FUNCTION_NO_MOD(natives::array_iterator_clone));
        _array_iterator->set_property(magic_methods::init, ALLOC_NATIVE_FUNCTION_NO_MOD(natives::array_iterator_init));
    }

    void NativeObjects::initialize_exception(Process* process) {
        _exception = process->get_heap().allocate<Exception>(process, _object);

        _exception->set_property(magic_methods::clone, ALLOC_NATIVE_FUNCTION_NO_MOD(natives::exception_clone));
        _exception->set_property(magic_methods::init, ALLOC_NATIVE_FUNCTION_NO_MOD(natives::exception_init));
    }

    void NativeObjects::initialize_number(Process* process) {
        _number = process->get_heap().allocate<Number>(process, _object);

        _number->set_property(magic_methods::neg, ALLOC_NATIVE_FUNCTION_NO_MOD(natives::number_neg));
        _number->set_property(magic_methods::add, ALLOC_NATIVE_FUNCTION_NO_MOD(natives::number_add));
        _number->set_property(magic_methods::sub, ALLOC_NATIVE_FUNCTION_NO_MOD(natives::number_sub));
        _number->set_property(magic_methods::mul, ALLOC_NATIVE_FUNCTION_NO_MOD(natives::number_mul));
        _number->set_property(magic_methods::div, ALLOC_NATIVE_FUNCTION_NO_MOD(natives::number_div));
        _number->set_property(magic_methods::mod, ALLOC_NATIVE_FUNCTION_NO_MOD(natives::number_mod));

        _number->set_property(magic_methods::iadd, ALLOC_NATIVE_FUNCTION_NO_MOD(natives::number_iadd));
        _number->set_property(magic_methods::isub, ALLOC_NATIVE_FUNCTION_NO_MOD(natives::number_isub));
        _number->set_property(magic_methods::imul, ALLOC_NATIVE_FUNCTION_NO_MOD(natives::number_imul));
        _number->set_property(magic_methods::idiv, ALLOC_NATIVE_FUNCTION_NO_MOD(natives::number_idiv));
        _number->set_property(magic_methods::imod, ALLOC_NATIVE_FUNCTION_NO_MOD(natives::number_imod));

        _number->set_property(magic_methods::eq, ALLOC_NATIVE_FUNCTION_NO_MOD(natives::number_eq));
        _number->set_property(magic_methods::neq, ALLOC_NATIVE_FUNCTION_NO_MOD(natives::number_neq));
        _number->set_property(magic_methods::lt, ALLOC_NATIVE_FUNCTION_NO_MOD(natives::number_lt));
        _number->set_property(magic_methods::gt, ALLOC_NATIVE_FUNCTION_NO_MOD(natives::number_gt));
        _number->set_property(magic_methods::lte, ALLOC_NATIVE_FUNCTION_NO_MOD(natives::number_lte));
        _number->set_property(magic_methods::gte, ALLOC_NATIVE_FUNCTION_NO_MOD(natives::number_gte));

        _number->set_property(magic_methods::bit_or, ALLOC_NATIVE_FUNCTION_NO_MOD(natives::number_bit_or));
        _number->set_property(magic_methods::bit_xor, ALLOC_NATIVE_FUNCTION_NO_MOD(natives::number_bit_xor));
        _number->set_property(magic_methods::bit_and, ALLOC_NATIVE_FUNCTION_NO_MOD(natives::number_bit_and));
        _number->set_property(magic_methods::bit_shl, ALLOC_NATIVE_FUNCTION_NO_MOD(natives::number_bit_shl));
        _number->set_property(magic_methods::bit_shr, ALLOC_NATIVE_FUNCTION_NO_MOD(natives::number_bit_shr));

        _number->set_property(magic_methods::clone, ALLOC_NATIVE_FUNCTION_NO_MOD(natives::number_clone));
        _number->set_property(magic_methods::init, ALLOC_NATIVE_FUNCTION_NO_MOD(natives::number_init));
    }

    void NativeObjects::initialize_string(Process* process) {
        _string = process->get_heap().allocate<String>(process, _object);

        _string->set_property(magic_methods::add, ALLOC_NATIVE_FUNCTION_NO_MOD(natives::string_add));

        _string->set_property(magic_methods::eq, ALLOC_NATIVE_FUNCTION_NO_MOD(natives::string_eq));
        _string->set_property(magic_methods::neq, ALLOC_NATIVE_FUNCTION_NO_MOD(natives::string_neq));
        _string->set_property(magic_methods::lt, ALLOC_NATIVE_FUNCTION_NO_MOD(natives::string_lt));
        _string->set_property(magic_methods::gt, ALLOC_NATIVE_FUNCTION_NO_MOD(natives::string_gt));
        _string->set_property(magic_methods::lte, ALLOC_NATIVE_FUNCTION_NO_MOD(natives::string_lte));
        _string->set_property(magic_methods::gte, ALLOC_NATIVE_FUNCTION_NO_MOD(natives::string_gte));

        _string->set_property(magic_methods::clone, ALLOC_NATIVE_FUNCTION_NO_MOD(natives::string_clone));

        _string->set_property("empty", ALLOC_NATIVE_FUNCTION_NO_MOD(natives::string_empty));
        _string->set_property("len", ALLOC_NATIVE_FUNCTION_NO_MOD(natives::string_length));
        _string->set_property("at", ALLOC_NATIVE_FUNCTION_NO_MOD(natives::string_at));
        _string->set_property("back", ALLOC_NATIVE_FUNCTION_NO_MOD(natives::string_back));
        _string->set_property("front", ALLOC_NATIVE_FUNCTION_NO_MOD(natives::string_front));
        _string->set_property("compare", ALLOC_NATIVE_FUNCTION_NO_MOD(natives::string_compare));
        _string->set_property("find", ALLOC_NATIVE_FUNCTION_NO_MOD(natives::string_find));  
        _string->set_property("substr", ALLOC_NATIVE_FUNCTION_NO_MOD(natives::string_substr));
        _string->set_property("format", ALLOC_NATIVE_FUNCTION_NO_MOD(natives::string_format));
        _string->set_property("split", ALLOC_NATIVE_FUNCTION_NO_MOD(natives::string_split));
        _string->set_property("append", ALLOC_NATIVE_FUNCTION_NO_MOD(natives::string_append));
    }

    void NativeObjects::initialize_booleans(Process* process) {
        _boolean = process->get_heap().allocate<Boolean>(process, _object);

        _boolean->set_property(magic_methods::eq, ALLOC_NATIVE_FUNCTION_NO_MOD(natives::boolean_eq));
        _boolean->set_property(magic_methods::neq, ALLOC_NATIVE_FUNCTION_NO_MOD(natives::boolean_neq));

        _boolean->set_property(magic_methods::clone, ALLOC_NATIVE_FUNCTION_NO_MOD(natives::boolean_clone));
        _boolean->set_property(magic_methods::init, ALLOC_NATIVE_FUNCTION_NO_MOD(natives::boolean_init));

        _true = process->get_heap().allocate<Boolean>(process, _boolean, true);
        _false = process->get_heap().allocate<Boolean>(process, _boolean, false);
     }

} // namespace emerald
