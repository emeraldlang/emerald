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

#include "emerald/globals.h"
#include "emerald/natives/array.h"
#include "emerald/natives/number.h"
#include "emerald/natives/string.h"
#include "emerald/magic_methods.h"

namespace emerald {

    Globals::Globals(Heap* heap)
        : _heap(heap) {
        initialize_object();
        initialize_array();
        initialize_number();
        initialize_string();
    }

    const HeapObject* Globals::get_object() const {
        return _object;
    }

    HeapObject* Globals::get_object() {
        return _object;
    }

    const Array* Globals::get_array() const {
        return _array;
    }

    Array* Globals::get_array() {
        return _array;
    }

    const Number* Globals::get_number() const {
        return _number;
    }

    Number* Globals::get_number() {
        return _number;
    }

    const String* Globals::get_string() const {
        return _string;
    }

    String* Globals::get_string() {
        return _string;
    }

    std::vector<HeapManaged*> Globals::get_roots() const {
        std::vector<HeapManaged*> roots;
        return roots;
    }

    void Globals::initialize_object() {
        _object = _heap->allocate<HeapObject>();
    }

    void Globals::initialize_array() {
        _array = _heap->allocate<Array>(_object);

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

    void Globals::initialize_number() {
        _number = _heap->allocate<Number>(_object);

        _number->set_property(magic_methods::add, natives::get_number_add());
        _number->set_property(magic_methods::sub, natives::get_number_sub());
        _number->set_property(magic_methods::mul, natives::get_number_mul());
        _number->set_property(magic_methods::div, natives::get_number_div());
        _number->set_property(magic_methods::mod, natives::get_number_mod());
        _number->set_property(magic_methods::prefix_inc, natives::get_number_prefix_inc());
        _number->set_property(magic_methods::prefix_dec, natives::get_number_prefix_dec());
        _number->set_property(magic_methods::postfix_inc, natives::get_number_postfix_inc());
        _number->set_property(magic_methods::postfix_dec, natives::get_number_postfix_dec());

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

    void Globals::initialize_string() {
        _string = _heap->allocate<String>(_object);

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
