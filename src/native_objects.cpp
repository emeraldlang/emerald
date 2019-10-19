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

#include "emerald/execution_context.h"
#include "emerald/native_objects.h"
#include "emerald/magic_methods.h"
#include "emerald/natives/array.h"
#include "emerald/natives/boolean.h"
#include "emerald/natives/number.h"
#include "emerald/natives/object.h"
#include "emerald/natives/string.h"

namespace emerald {

    NativeObjects::NativeObjects(ExecutionContext* context) {
        initialize_object(context);
        initialize_array(context);
        initialize_number(context);
        initialize_string(context);

        initialize_booleans(context);
        _null = context->get_heap().allocate<Null>(context);
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

    void NativeObjects::initialize_object(ExecutionContext* context) {
        Heap& heap = context->get_heap();

        _object = heap.allocate<Object>(context);

        _object->set_property(magic_methods::eq, heap.allocate<NativeFunction>(context, natives::object_eq));
        _object->set_property(magic_methods::neq, heap.allocate<NativeFunction>(context, natives::object_neq));

        _object->set_property(magic_methods::str, heap.allocate<NativeFunction>(context, natives::object_str));
        _object->set_property(magic_methods::boolean, heap.allocate<NativeFunction>(context, natives::object_boolean));

        _object->set_property(magic_methods::clone, heap.allocate<NativeFunction>(context, natives::object_clone));
        _object->set_property(magic_methods::init, heap.allocate<NativeFunction>(context, natives::object_init));
    }

    void NativeObjects::initialize_array(ExecutionContext* context) {
        Heap& heap = context->get_heap();

        _array = heap.allocate<Array>(context, _object);

        _array->set_property(magic_methods::eq, heap.allocate<NativeFunction>(context, natives::array_eq));
        _array->set_property(magic_methods::neq, heap.allocate<NativeFunction>(context, natives::array_neq));

        _array->set_property(magic_methods::clone, heap.allocate<NativeFunction>(context, natives::array_clone));

        _array->set_property("at", heap.allocate<NativeFunction>(context, natives::array_at));
        _array->set_property("front", heap.allocate<NativeFunction>(context, natives::array_front));
        _array->set_property("back", heap.allocate<NativeFunction>(context, natives::array_back));
        _array->set_property("empty", heap.allocate<NativeFunction>(context, natives::array_empty));
        _array->set_property("size", heap.allocate<NativeFunction>(context, natives::array_size));
        _array->set_property("clear", heap.allocate<NativeFunction>(context, natives::array_clear));
        _array->set_property("push", heap.allocate<NativeFunction>(context, natives::array_push));
        _array->set_property("pop", heap.allocate<NativeFunction>(context, natives::array_pop));
    }

    void NativeObjects::initialize_number(ExecutionContext* context) {
        Heap& heap = context->get_heap();

        _number = context->get_heap().allocate<Number>(context, _object);

        _number->set_property(magic_methods::neg, heap.allocate<NativeFunction>(context, natives::number_neg));
        _number->set_property(magic_methods::add, heap.allocate<NativeFunction>(context, natives::number_add));
        _number->set_property(magic_methods::sub, heap.allocate<NativeFunction>(context, natives::number_sub));
        _number->set_property(magic_methods::mul, heap.allocate<NativeFunction>(context, natives::number_mul));
        _number->set_property(magic_methods::div, heap.allocate<NativeFunction>(context, natives::number_div));
        _number->set_property(magic_methods::mod, heap.allocate<NativeFunction>(context, natives::number_mod));

        _number->set_property(magic_methods::iadd, heap.allocate<NativeFunction>(context, natives::number_iadd));
        _number->set_property(magic_methods::isub, heap.allocate<NativeFunction>(context, natives::number_isub));
        _number->set_property(magic_methods::imul, heap.allocate<NativeFunction>(context, natives::number_imul));
        _number->set_property(magic_methods::idiv, heap.allocate<NativeFunction>(context, natives::number_idiv));
        _number->set_property(magic_methods::imod, heap.allocate<NativeFunction>(context, natives::number_imod));

        _number->set_property(magic_methods::eq, heap.allocate<NativeFunction>(context, natives::number_eq));
        _number->set_property(magic_methods::neq, heap.allocate<NativeFunction>(context, natives::number_neq));
        _number->set_property(magic_methods::lt, heap.allocate<NativeFunction>(context, natives::number_lt));
        _number->set_property(magic_methods::gt, heap.allocate<NativeFunction>(context, natives::number_gt));
        _number->set_property(magic_methods::lte, heap.allocate<NativeFunction>(context, natives::number_lte));
        _number->set_property(magic_methods::gte, heap.allocate<NativeFunction>(context, natives::number_gte));

        _number->set_property(magic_methods::bit_or, heap.allocate<NativeFunction>(context, natives::number_bit_or));
        _number->set_property(magic_methods::bit_xor, heap.allocate<NativeFunction>(context, natives::number_bit_xor));
        _number->set_property(magic_methods::bit_and, heap.allocate<NativeFunction>(context, natives::number_bit_and));
        _number->set_property(magic_methods::bit_shl, heap.allocate<NativeFunction>(context, natives::number_bit_shl));
        _number->set_property(magic_methods::bit_shr, heap.allocate<NativeFunction>(context, natives::number_bit_shr));

        _number->set_property(magic_methods::clone, heap.allocate<NativeFunction>(context, natives::number_clone));
    }

    void NativeObjects::initialize_string(ExecutionContext* context) {
        Heap& heap = context->get_heap();

        _string = context->get_heap().allocate<String>(context, _object);

        _string->set_property(magic_methods::add, heap.allocate<NativeFunction>(context, natives::string_add));

        _string->set_property(magic_methods::eq, heap.allocate<NativeFunction>(context, natives::string_eq));
        _string->set_property(magic_methods::neq, heap.allocate<NativeFunction>(context, natives::string_neq));
        _string->set_property(magic_methods::lt, heap.allocate<NativeFunction>(context, natives::string_lt));
        _string->set_property(magic_methods::gt, heap.allocate<NativeFunction>(context, natives::string_gt));
        _string->set_property(magic_methods::lte, heap.allocate<NativeFunction>(context, natives::string_lte));
        _string->set_property(magic_methods::gte, heap.allocate<NativeFunction>(context, natives::string_gte));

        _string->set_property(magic_methods::clone, heap.allocate<NativeFunction>(context, natives::string_clone));

        _string->set_property("empty", heap.allocate<NativeFunction>(context, natives::string_empty));
        _string->set_property("len", heap.allocate<NativeFunction>(context, natives::string_length));
        _string->set_property("at", heap.allocate<NativeFunction>(context, natives::string_at));
        _string->set_property("back", heap.allocate<NativeFunction>(context, natives::string_back));
        _string->set_property("front", heap.allocate<NativeFunction>(context, natives::string_front));
        _string->set_property("compare", heap.allocate<NativeFunction>(context, natives::string_compare));
        _string->set_property("find", heap.allocate<NativeFunction>(context, natives::string_find));
        _string->set_property("format", heap.allocate<NativeFunction>(context, natives::string_format));
        _string->set_property("substr", heap.allocate<NativeFunction>(context, natives::string_substr));
    }

    void NativeObjects::initialize_booleans(ExecutionContext* context) {
        Heap& heap = context->get_heap();

        _boolean = context->get_heap().allocate<Boolean>(context, _object);

        _boolean->set_property(magic_methods::eq, heap.allocate<NativeFunction>(context, natives::boolean_eq));
        _boolean->set_property(magic_methods::neq, heap.allocate<NativeFunction>(context, natives::boolean_neq));

        _boolean->set_property(magic_methods::clone, heap.allocate<NativeFunction>(context, natives::boolean_clone));

        _true = context->get_heap().allocate<Boolean>(context, _boolean, true);
        _false = context->get_heap().allocate<Boolean>(context, _boolean, false);
     }

} // namespace emerald
