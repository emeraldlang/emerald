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

namespace emerald {

    Object::Object()
        : _parent(nullptr) {}

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

        if (_parent) {
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

    bool Object::set_property(const std::string& key, Object* value) {
        _properties[key] = value;
        return true;
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

    Object* NativeFunction::invoke(Heap* heap, NativeObjects* native_objects, const std::vector<Object*>& args) {
        return _callable(heap, native_objects, args);
    }

    Object* NativeFunction::operator()(Heap* heap, NativeObjects* native_objects, const std::vector<Object*>& args) {
        return _callable(heap, native_objects, args);
    }

    Null::Null()
        : Object() {}

    bool Null::as_bool() const {
        return false;
    }

    std::string Null::as_str() const {
        return "None";
    }

    Object* Null::get_property(const std::string&) {
        return nullptr;
    }

    Object* Null::get_own_property(const std::string&) {
        return nullptr;
    }

    bool Null::has_property(const std::string&) const {
        return false;
    }

    bool Null::has_own_property(const std::string&) const {
        return false;
    }

    bool Null::set_property(const std::string&, Object*) {
        return false;
    }

    HeapObject::HeapObject()
        : Object(), 
        HeapManaged() {}

    HeapObject::HeapObject(Object* parent)
        : Object(parent), 
        HeapManaged() {}

    bool HeapObject::as_bool() const {
        return true;
    }
    
    std::string HeapObject::as_str() const {
        return "<object>";
    }

    void HeapObject::reach() {
        for (std::pair<std::string, Object*> pair : get_properties()) {
            if (HeapObject* heap_obj = dynamic_cast<HeapObject*>(pair.second)) {
                heap_obj->mark();
            }
        }
    }

    Array::Array()
        : HeapObject() {}

    Array::Array(Object* parent)
        : HeapObject(parent) {}

    bool Array::as_bool() const {
        return _value.size() > 0;
    }
    
    std::string Array::as_str() const {
        std::string str = "[";
        for (Object* obj : _value) {
            str += obj->as_str();
            if (obj != _value.back()) {
                str += ",";
            }
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

    Boolean::Boolean(bool value)
        : HeapObject(),
        _value(value) {}

    Boolean::Boolean(Object* parent, bool value)
        : HeapObject(parent),
        _value(value) {}

    bool Boolean::as_bool() const {
        return _value;
    }

    std::string Boolean::as_str() const {
        return (_value) ? "True" : "False";
    }

    bool Boolean::get_value() const {
        return _value;
    }

    Exception::Exception(const std::string& message)
        : HeapObject(),
        _message(message) {}

    Exception::Exception(Object* parent, const std::string& message)
        : HeapObject(parent),
        _message(message) {}

    std::string Exception::as_str() const {
        return _message;
    }

    const std::string& Exception::get_message() const {
        return _message;
    }

    Function::Function(std::shared_ptr<const Code> code)
        : HeapObject(),
        _code(code) {}

    Function::Function(Object* parent, std::shared_ptr<const Code> code)
        : HeapObject(parent),
        _code(code) {}

    std::string Function::as_str() const {
        return fmt::format("<function {}>", _code->get_label());
    }

    std::shared_ptr<const Code> Function::get_code() const {
        return _code;
    }

    Number::Number(double value)
        : HeapObject(),
        _value(value) {}

    Number::Number(Object* parent, double value)
        : HeapObject(parent),
        _value(value) {}

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

    String::String(const std::string& value)
        : HeapObject(),
        _value(value) {}

    String::String(Object* parent, const std::string& value)
        : HeapObject(parent),
        _value(value) {}

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

} // namespace emerald
