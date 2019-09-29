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

#include "emerald/native_prototypes.h"
#include "emerald/magic_methods.h"
#include "emerald/natives/array.h"
#include "emerald/natives/boolean.h"
#include "emerald/natives/number.h"
#include "emerald/natives/object.h"
#include "emerald/natives/string.h"

namespace emerald {

    NativePrototypes::NativePrototypes(Heap* heap) {
        initialize_object(heap);
        initialize_boolean(heap);
        initialize_array(heap);
        initialize_number(heap);
        initialize_string(heap);
    }

    const HeapObject* NativePrototypes::get_object_prototype() const {
        return _object;
    }

    HeapObject* NativePrototypes::get_object_prototype() {
        return _object;
    }

    const Array* NativePrototypes::get_array_prototype() const {
        return _array;
    }

    Array* NativePrototypes::get_array_prototype() {
        return _array;
    }

    const Boolean* NativePrototypes::get_boolean_prototype() const {
        return _boolean;
    }

    Boolean* NativePrototypes::get_boolean_prototype() {
        return _boolean;
    }

    const Number* NativePrototypes::get_number_prototype() const {
        return _number;
    }

    Number* NativePrototypes::get_number_prototype() {
        return _number;
    }

    const String* NativePrototypes::get_string_prototype() const {
        return _string;
    }

    String* NativePrototypes::get_string_prototype() {
        return _string;
    }

    void NativePrototypes::initialize_object(Heap* heap) {
        _object = heap->allocate<HeapObject>();

        _object->set_property(magic_methods::eq, natives::get_object_eq());
        _object->set_property(magic_methods::neq, natives::get_object_neq());

        _object->set_property(magic_methods::clone, natives::get_object_clone());
    }

    void NativePrototypes::initialize_array(Heap* heap) {
        _array = heap->allocate<Array>(_object);

        _array->set_property(magic_methods::eq, natives::get_array_eq());
        _array->set_property(magic_methods::neq, natives::get_array_neq());

        _array->set_property(magic_methods::clone, natives::get_array_clone());

        _array->set_property("at", natives::get_array_at());
        _array->set_property("front", natives::get_array_front());
        _array->set_property("back", natives::get_array_back());
        _array->set_property("empty", natives::get_array_empty());
        _array->set_property("size", natives::get_array_size());
        _array->set_property("clear", natives::get_array_clear());
        _array->set_property("push", natives::get_array_push());
        _array->set_property("pop", natives::get_array_pop());
    }

    void NativePrototypes::initialize_boolean(Heap* heap) {
        _boolean = heap->allocate<Boolean>(_object);

        _boolean->set_property(magic_methods::eq, natives::get_boolean_eq());
        _boolean->set_property(magic_methods::neq, natives::get_boolean_neq());

        _boolean->set_property(magic_methods::clone, natives::get_boolean_clone());
     }

    void NativePrototypes::initialize_number(Heap* heap) {
        _number = heap->allocate<Number>(_object);

        _number->set_property(magic_methods::neg, natives::get_number_neg());

        _number->set_property(magic_methods::add, natives::get_number_add());
        _number->set_property(magic_methods::sub, natives::get_number_sub());
        _number->set_property(magic_methods::mul, natives::get_number_mul());
        _number->set_property(magic_methods::div, natives::get_number_div());
        _number->set_property(magic_methods::mod, natives::get_number_mod());

        _number->set_property(magic_methods::iadd, natives::get_number_iadd());
        _number->set_property(magic_methods::isub, natives::get_number_isub());
        _number->set_property(magic_methods::imul, natives::get_number_imul());
        _number->set_property(magic_methods::idiv, natives::get_number_idiv());
        _number->set_property(magic_methods::imod, natives::get_number_imod());

        _number->set_property(magic_methods::eq, natives::get_number_eq());
        _number->set_property(magic_methods::neq, natives::get_number_neq());
        _number->set_property(magic_methods::lt, natives::get_number_lt());
        _number->set_property(magic_methods::gt, natives::get_number_gt());
        _number->set_property(magic_methods::lte, natives::get_number_lte());
        _number->set_property(magic_methods::gte, natives::get_number_gte());

        _number->set_property(magic_methods::bit_or, natives::get_number_bit_or());
        _number->set_property(magic_methods::bit_xor, natives::get_number_bit_xor());
        _number->set_property(magic_methods::bit_and, natives::get_number_bit_and());
        _number->set_property(magic_methods::bit_shl, natives::get_number_bit_shl());
        _number->set_property(magic_methods::bit_shr, natives::get_number_bit_shr());

        _number->set_property(magic_methods::clone, natives::get_number_clone());
    }

    void NativePrototypes::initialize_string(Heap* heap) {
        _string = heap->allocate<String>(_object);

        _string->set_property(magic_methods::add, natives::get_string_add());

        _string->set_property(magic_methods::eq, natives::get_string_eq());
        _string->set_property(magic_methods::neq, natives::get_string_neq());
        _string->set_property(magic_methods::lt, natives::get_string_lt());
        _string->set_property(magic_methods::gt, natives::get_string_gt());
        _string->set_property(magic_methods::lte, natives::get_string_lte());
        _string->set_property(magic_methods::gte, natives::get_string_gte());

        _string->set_property(magic_methods::clone, natives::get_string_clone());

        _string->set_property("empty", natives::get_string_empty());
        _string->set_property("len", natives::get_string_length());
        _string->set_property("at", natives::get_string_at());
        _string->set_property("back", natives::get_string_back());
        _string->set_property("front", natives::get_string_front());
        _string->set_property("compare", natives::get_string_compare());
        _string->set_property("find", natives::get_string_find());
        _string->set_property("format", natives::get_string_format());
        _string->set_property("substr", natives::get_string_substr());
    }

} // namespace emerald
