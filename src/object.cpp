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

#include "fmt/format.h"

#include "emerald/object.h"
#include "emerald/natives/number.h"
#include "emerald/natives/string.h"
#include "emerald/magic_methods.h"

namespace emerald {

    Object::Object() {}

    Object::Object(Object* parent)
        : _parent(parent) {}

    Object::~Object() {}

    bool Object::as_bool() const {
        return true;
    }

    Object* Object::get_parent() const {
        return _parent;
    }

    const std::unordered_map<std::string, Object*>& Object::get_properties() const {
        return _properties;
    }

    Object* Object::get_property(const std::string& key) {
        if (Object* property = get_own_property(key)) {
            return property;
        }

        if (_parent != nullptr) {
            return _parent->get_property(key);
        }

        return nullptr;
    }

    Object* Object::get_own_property(const std::string& key) {
        if (_properties.find(key) != _properties.end()) {
            return _properties[key];
        }

        return nullptr;
    }

    bool Object::has_property(const std::string& key) const {
        if (has_own_property(key)) {
            return true;
        }

        if (_parent != nullptr) {
            return _parent->has_property(key);
        }

        return false;
    }

    bool Object::has_own_property(const std::string& key) const {
        return _properties.find(key) != _properties.end();
    }

    void Object::set_property(const std::string& key, Object* value) {
        _properties[key] = value;
    }

    NativeFunction::NativeFunction(Callable callable)
        : Object(),
        _callable(callable) {}

    NativeFunction::NativeFunction(Object* parent, Callable callable)
        : Object(parent),
        _callable(callable) {}
    
    std::string NativeFunction::as_str() const {
        return "<native_function>"; // Include a name ?
    }
    const NativeFunction::Callable& NativeFunction::get_callable() const {
        return _callable;
    }

    Object* NativeFunction::invoke(Heap* heap, const std::vector<Object*>& args) {
        return _callable(heap, args);
    }

    Object* NativeFunction::operator()(Heap* heap, const std::vector<Object*>& args) {
        return _callable(heap, args);
    }

    HeapObject::HeapObject(Heap* heap)
        : Object(), 
        HeapManaged(heap) {}

    HeapObject::HeapObject(Heap* heap, Object* parent)
        : Object(parent), 
        HeapManaged(heap) {}

    void HeapObject::reach() {
        for (std::pair<std::string, Object*> pair : get_properties()) {
            if (HeapObject* heap_obj = dynamic_cast<HeapObject*>(pair.second)) {
                heap_obj->mark();
            }
        }
    }

    Array::Array(Heap* heap)
        : HeapObject(heap) {}

    Array::Array(Heap* heap, Object* parent)
        : HeapObject(heap, parent) {}

    bool Array::as_bool() const {
        return _value.size() > 0;
    }
    
    std::string Array::as_str() const {
        std::string str = "[";
        for (Object* obj : _value) {
            str += obj->as_str();
        }
        str += "]";
        return str;
    }

    std::vector<Object*>& Array::get_value() {
        return _value;
    }

    const std::vector<Object*>& Array::get_value() const {
        return _value;
    }

    Boolean::Boolean(Heap* heap, bool value)
        : HeapObject(heap),
        _value(value) {}

    Boolean::Boolean(Heap* heap, Object* parent, bool value)
        : HeapObject(heap, parent),
        _value(value) {}

    bool Boolean::as_bool() const {
        return _value;
    }

    std::string Boolean::as_str() const {
        return (_value) ? "true" : "false";
    }

    bool Boolean::get_value() const {
        return _value;
    }

    Exception::Exception(Heap* heap, const std::string& message)
        : HeapObject(heap),
        _message(message) {}

    Exception::Exception(Heap* heap, Object* parent, const std::string& message)
        : HeapObject(heap, parent),
        _message(message) {}

    std::string Exception::as_str() const {
        return _message;
    }

    const std::string& Exception::get_message() const {
        return _message;
    }

    Function::Function(Heap* heap, std::shared_ptr<const Code> code)
        : HeapObject(heap),
        _code(code) {}

    Function::Function(Heap* heap, Object* parent, std::shared_ptr<const Code> code)
        : HeapObject(heap, parent),
        _code(code) {}

    std::string Function::as_str() const {
        return fmt::format("<function {}>", _code->get_label());
    }

    std::shared_ptr<const Code> Function::get_code() const {
        return _code;
    }

    Number::Number(Heap* heap, double value)
        : HeapObject(heap),
        _value(value) {
        initialize_properties();
    }

    Number::Number(Heap* heap, Object* parent, double value)
        : HeapObject(heap, parent),
        _value(value) {
        initialize_properties();
    }

    bool Number::as_bool() const {
        return _value != 0;
    }

    std::string Number::as_str() const {
        return fmt::format("{}", _value);
    }

    double Number::get_value() const {
        return _value;
    }

    void Number::set_value(double val) {
        _value = val;
    }

    void Number::increment() {
        _value++;
    }

    void Number::decrement() {
        _value--;
    }

    void Number::initialize_properties() {
        set_property(magic_methods::add, natives::get_number_add());
        set_property(magic_methods::sub, natives::get_number_sub());
        set_property(magic_methods::mul, natives::get_number_mul());
        set_property(magic_methods::div, natives::get_number_div());
        set_property(magic_methods::mod, natives::get_number_mod());
        set_property(magic_methods::prefix_inc, natives::get_number_prefix_inc());
        set_property(magic_methods::prefix_dec, natives::get_number_prefix_dec());
        set_property(magic_methods::postfix_inc, natives::get_number_postfix_inc());
        set_property(magic_methods::postfix_dec, natives::get_number_postfix_dec());

        set_property(magic_methods::eq, natives::get_number_eq());
        set_property(magic_methods::neq, natives::get_number_neq());
        set_property(magic_methods::lt, natives::get_number_lt());
        set_property(magic_methods::gt, natives::get_number_gt());
        set_property(magic_methods::lte, natives::get_number_lte());
        set_property(magic_methods::gte, natives::get_number_gte());

        set_property(magic_methods::bit_or, natives::get_number_bit_or());
        set_property(magic_methods::bit_xor, natives::get_number_bit_xor());
        set_property(magic_methods::bit_and, natives::get_number_bit_and());
        set_property(magic_methods::bit_shl, natives::get_number_bit_shl());
        set_property(magic_methods::bit_shr, natives::get_number_bit_shr());
    }

    String::String(Heap* heap, const std::string& value)
        : HeapObject(heap),
        _value(value) {
        initialize_properties();
    }

    String::String(Heap* heap, Object* parent, const std::string& value)
        : HeapObject(heap, parent),
        _value(value) {
        initialize_properties();
    }

    bool String::as_bool() const {
        return _value.size() > 0;
    }

    std::string String::as_str() const {
        return _value;
    }

    std::string& String::get_value() {
        return _value;
    }

    const std::string& String::get_value() const {
        return _value;
    }

    void String::initialize_properties() {
        set_property(magic_methods::add, natives::get_string_add());

        set_property(magic_methods::eq, natives::get_string_eq());
        set_property(magic_methods::neq, natives::get_string_neq());
        set_property(magic_methods::lt, natives::get_string_lt());
        set_property(magic_methods::gt, natives::get_string_gt());
        set_property(magic_methods::lte, natives::get_string_lte());
        set_property(magic_methods::gte, natives::get_string_gte());

        set_property("empty", natives::get_string_empty());
        set_property("len", natives::get_string_length());
        set_property("at", natives::get_string_at());
        set_property("back", natives::get_string_back());
        set_property("front", natives::get_string_front());
        set_property("compare", natives::get_string_compare());
        set_property("find", natives::get_string_find());
        set_property("format", natives::get_string_format());
        set_property("substr", natives::get_string_substr());
    }

} // namespace emerald
