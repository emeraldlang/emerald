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
#include "emerald/objectutils.h"

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
        _object = context->get_heap().allocate<Object>(context);

        _object->set_property(magic_methods::eq, ALLOC_NATIVE_FUNCTION(natives::object_eq));
        _object->set_property(magic_methods::neq, ALLOC_NATIVE_FUNCTION(natives::object_neq));

        _object->set_property(magic_methods::str, ALLOC_NATIVE_FUNCTION(natives::object_str));
        _object->set_property(magic_methods::boolean, ALLOC_NATIVE_FUNCTION(natives::object_boolean));

        _object->set_property(magic_methods::clone, ALLOC_NATIVE_FUNCTION(natives::object_clone));
        _object->set_property(magic_methods::init, ALLOC_NATIVE_FUNCTION(natives::object_init));

        _object->set_property("keys", ALLOC_NATIVE_FUNCTION(natives::object_keys));
        _object->set_property("get_prop", ALLOC_NATIVE_FUNCTION(natives::object_get_prop));
        _object->set_property("set_prop", ALLOC_NATIVE_FUNCTION(natives::object_set_prop));
    }

    void NativeObjects::initialize_array(ExecutionContext* context) {
        _array = context->get_heap().allocate<Array>(context, _object);

        _array->set_property(magic_methods::eq, ALLOC_NATIVE_FUNCTION(natives::array_eq));
        _array->set_property(magic_methods::neq, ALLOC_NATIVE_FUNCTION(natives::array_neq));

        _array->set_property(magic_methods::clone, ALLOC_NATIVE_FUNCTION(natives::array_clone));

        _array->set_property("at", ALLOC_NATIVE_FUNCTION(natives::array_at));
        _array->set_property("front", ALLOC_NATIVE_FUNCTION(natives::array_front));
        _array->set_property("back", ALLOC_NATIVE_FUNCTION(natives::array_back));
        _array->set_property("empty", ALLOC_NATIVE_FUNCTION(natives::array_empty));
        _array->set_property("size", ALLOC_NATIVE_FUNCTION(natives::array_size));
        _array->set_property("clear", ALLOC_NATIVE_FUNCTION(natives::array_clear));
        _array->set_property("push", ALLOC_NATIVE_FUNCTION(natives::array_push));
        _array->set_property("pop", ALLOC_NATIVE_FUNCTION(natives::array_pop));
        _array->set_property("join", ALLOC_NATIVE_FUNCTION(natives::array_join));
    }

    void NativeObjects::initialize_number(ExecutionContext* context) {
        _number = context->get_heap().allocate<Number>(context, _object);

        _number->set_property(magic_methods::neg, ALLOC_NATIVE_FUNCTION(natives::number_neg));
        _number->set_property(magic_methods::add, ALLOC_NATIVE_FUNCTION(natives::number_add));
        _number->set_property(magic_methods::sub, ALLOC_NATIVE_FUNCTION(natives::number_sub));
        _number->set_property(magic_methods::mul, ALLOC_NATIVE_FUNCTION(natives::number_mul));
        _number->set_property(magic_methods::div, ALLOC_NATIVE_FUNCTION(natives::number_div));
        _number->set_property(magic_methods::mod, ALLOC_NATIVE_FUNCTION(natives::number_mod));

        _number->set_property(magic_methods::iadd, ALLOC_NATIVE_FUNCTION(natives::number_iadd));
        _number->set_property(magic_methods::isub, ALLOC_NATIVE_FUNCTION(natives::number_isub));
        _number->set_property(magic_methods::imul, ALLOC_NATIVE_FUNCTION(natives::number_imul));
        _number->set_property(magic_methods::idiv, ALLOC_NATIVE_FUNCTION(natives::number_idiv));
        _number->set_property(magic_methods::imod, ALLOC_NATIVE_FUNCTION(natives::number_imod));

        _number->set_property(magic_methods::eq, ALLOC_NATIVE_FUNCTION(natives::number_eq));
        _number->set_property(magic_methods::neq, ALLOC_NATIVE_FUNCTION(natives::number_neq));
        _number->set_property(magic_methods::lt, ALLOC_NATIVE_FUNCTION(natives::number_lt));
        _number->set_property(magic_methods::gt, ALLOC_NATIVE_FUNCTION(natives::number_gt));
        _number->set_property(magic_methods::lte, ALLOC_NATIVE_FUNCTION(natives::number_lte));
        _number->set_property(magic_methods::gte, ALLOC_NATIVE_FUNCTION(natives::number_gte));

        _number->set_property(magic_methods::bit_or, ALLOC_NATIVE_FUNCTION(natives::number_bit_or));
        _number->set_property(magic_methods::bit_xor, ALLOC_NATIVE_FUNCTION(natives::number_bit_xor));
        _number->set_property(magic_methods::bit_and, ALLOC_NATIVE_FUNCTION(natives::number_bit_and));
        _number->set_property(magic_methods::bit_shl, ALLOC_NATIVE_FUNCTION(natives::number_bit_shl));
        _number->set_property(magic_methods::bit_shr, ALLOC_NATIVE_FUNCTION(natives::number_bit_shr));

        _number->set_property(magic_methods::clone, ALLOC_NATIVE_FUNCTION(natives::number_clone));
    }

    void NativeObjects::initialize_string(ExecutionContext* context) {
        _string = context->get_heap().allocate<String>(context, _object);

        _string->set_property(magic_methods::add, ALLOC_NATIVE_FUNCTION(natives::string_add));

        _string->set_property(magic_methods::eq, ALLOC_NATIVE_FUNCTION(natives::string_eq));
        _string->set_property(magic_methods::neq, ALLOC_NATIVE_FUNCTION(natives::string_neq));
        _string->set_property(magic_methods::lt, ALLOC_NATIVE_FUNCTION(natives::string_lt));
        _string->set_property(magic_methods::gt, ALLOC_NATIVE_FUNCTION(natives::string_gt));
        _string->set_property(magic_methods::lte, ALLOC_NATIVE_FUNCTION(natives::string_lte));
        _string->set_property(magic_methods::gte, ALLOC_NATIVE_FUNCTION(natives::string_gte));

        _string->set_property(magic_methods::clone, ALLOC_NATIVE_FUNCTION(natives::string_clone));

        _string->set_property("empty", ALLOC_NATIVE_FUNCTION(natives::string_empty));
        _string->set_property("len", ALLOC_NATIVE_FUNCTION(natives::string_length));
        _string->set_property("at", ALLOC_NATIVE_FUNCTION(natives::string_at));
        _string->set_property("back", ALLOC_NATIVE_FUNCTION(natives::string_back));
        _string->set_property("front", ALLOC_NATIVE_FUNCTION(natives::string_front));
        _string->set_property("compare", ALLOC_NATIVE_FUNCTION(natives::string_compare));
        _string->set_property("find", ALLOC_NATIVE_FUNCTION(natives::string_find));  
        _string->set_property("substr", ALLOC_NATIVE_FUNCTION(natives::string_substr));
        _string->set_property("format", ALLOC_NATIVE_FUNCTION(natives::string_format));
        _string->set_property("split", ALLOC_NATIVE_FUNCTION(natives::string_split));
        _string->set_property("append", ALLOC_NATIVE_FUNCTION(natives::string_append));
    }

    void NativeObjects::initialize_booleans(ExecutionContext* context) {
        _boolean = context->get_heap().allocate<Boolean>(context, _object);

        _boolean->set_property(magic_methods::eq, ALLOC_NATIVE_FUNCTION(natives::boolean_eq));
        _boolean->set_property(magic_methods::neq, ALLOC_NATIVE_FUNCTION(natives::boolean_neq));

        _boolean->set_property(magic_methods::clone, ALLOC_NATIVE_FUNCTION(natives::boolean_clone));

        _true = context->get_heap().allocate<Boolean>(context, _boolean, true);
        _false = context->get_heap().allocate<Boolean>(context, _boolean, false);
     }

} // namespace emerald
