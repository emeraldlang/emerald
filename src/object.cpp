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

#include <algorithm>

#include "fmt/format.h"

#include "emerald/execution_context.h"
#include "emerald/interpreter.h"
#include "emerald/native_frame.h"
#include "emerald/magic_methods.h"
#include "emerald/module.h"
#include "emerald/object.h"
#include "emerald/objectutils.h"

namespace emerald {

    Object::Object(ExecutionContext* context)
        : _context(context), 
        _parent(nullptr) {}

    Object::Object(ExecutionContext* context, Object* parent)
        : _context(context), 
        _parent(parent) {}

    Object::~Object() {}

    bool Object::as_bool() const {
        return true;
    }
    
    std::string Object::as_str() const {
        return "<object>";
    }

    ExecutionContext* Object::get_context() const {
        return _context;
    }

    Object* Object::get_parent() const {
        return _parent;
    }

    const std::unordered_map<std::string, Object*>& Object::get_properties() const {
        return _properties;
    }

    const Object* Object::get_property(const std::string& key) const {
        if (const Object* property = get_own_property(key)) {
            return property;
        }

        if (_parent) {
            return _parent->get_property(key);
        }

        return nullptr;
    }
    
    const Object* Object::get_own_property(const std::string& key) const {
        if (has_own_property(key)) {
            return _properties.at(key);
        }

        return nullptr;
    }

    Object* Object::get_property(const std::string& key) {
        return const_cast<Object*>(static_cast<const Object&>(*this).get_property(key));
    }

    Object* Object::get_own_property(const std::string& key) {
        return const_cast<Object*>(static_cast<const Object&>(*this).get_own_property(key));
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

    void Object::reach() {
        if (_parent != nullptr) {
            _parent->mark();
        }

        for (std::pair<std::string, Object*> pair : get_properties()) {
            pair.second->mark();
        }
    }

    Array::Array(ExecutionContext* context, const std::vector<Object*>& value)
        : Object(context, context->get_native_objects().get_array_prototype()),
        _value(value) {}

    Array::Array(ExecutionContext* context, Object* parent, const std::vector<Object*>& value)
        : Object(context, parent),
        _value(value) {}

    bool Array::as_bool() const {
        return _value.size() > 0;
    }
    
    std::string Array::as_str() const {
        return "[" +
            objectutils::join_range(_value.begin(), _value.end(), ",", get_context())
        + "]";
    }

    void Array::init(Object* iterator) {
        objectutils::ObjectIterator iter = objectutils::ObjectIterator(get_context(), iterator);
        while (!iter.done()) {
            _value.push_back(iter.cur());
            iter.next();
        }
    }

    Object* Array::at(size_t i) const {
        if (i >= _value.size()) {
            return nullptr;
        }

        return _value[i];
    }

    Object* Array::front() const {
        return _value.front();
    }

    Object* Array::back() const {
        return _value.back();
    }

    Boolean* Array::empty() const {
        return BOOLEAN_IN_CTX(_value.empty(), get_context());
    }

    Number* Array::size() {
        return ALLOC_NUMBER_IN_CTX(_value.size(), get_context());
    }

    void Array::clear() {
        _value.clear();
    }

    void Array::push(Object* obj) {
        _value.push_back(obj);
    }

    Object* Array::pop() {
        Object* obj = _value.back();
        _value.pop_back();
        return obj;
    }

    String* Array::join(String* seperator) {
        ExecutionContext* context = get_context();
        return ALLOC_STRING(objectutils::join_range(
            _value.begin(),
            _value.end(),
            seperator->get_value(),
            context));
    }

    bool Array::operator==(const Array& other) const {
        return objectutils::compare_range(
            _value.begin(),
            _value.end(),
            other._value.begin(),
            get_context());
    }

    bool Array::operator!=(const Array& other) const {
        return !(*this == other);
    }

    void Array::reach() {
        Object::reach();

        for (Object* obj : _value) {
            obj->mark();
        }
    }

    Array::Iterator::Iterator(ExecutionContext* context)
        : Object(context, context->get_native_objects().get_array_iterator_prototype()),
        _arr(nullptr),
        _i(0) {}

    Array::Iterator::Iterator(ExecutionContext* context, Object* parent)
        : Object(context, parent),
        _arr(nullptr),
        _i(0) {}

    std::string Array::Iterator::as_str() const {
        return "<array_iterator>";
    }

    void Array::Iterator::init(Array* arr) {
        _arr = arr;
    }

    Object* Array::Iterator::cur() const {
        return _arr->at(_i);
    }

    Boolean* Array::Iterator::done() const {
        return BOOLEAN_IN_CTX(_i == _arr->size()->get_value(), get_context());
    }

    Object* Array::Iterator::next() {
        if (_i == _arr->size()->get_value()) {
            return _arr->back();
        }

        return _arr->at(++_i);
    }

    void Array::Iterator::reach() {
        Object::reach();

        if (_arr) {
            _arr->mark();
        }
    }

    Boolean::Boolean(ExecutionContext* context, bool value)
        : Object(context, context->get_native_objects().get_boolean_prototype()),
        _value(value) {}

    Boolean::Boolean(ExecutionContext* context, Object* parent, bool value)
        : Object(context, parent),
        _value(value) {}

    bool Boolean::as_bool() const {
        return _value;
    }

    std::string Boolean::as_str() const {
        return (_value) ? "True" : "False";
    }

    void Boolean::init(Boolean* val) {
        _value = val->get_native_value();
    }

    bool Boolean::get_native_value() const {
        return _value;
    }

    Boolean* Boolean::eq(Boolean* other) const {
        return BOOLEAN_IN_CTX(_value == other->_value, get_context());
    }

    Boolean* Boolean::neq(Boolean* other) const {
        return BOOLEAN_IN_CTX(_value != other->_value, get_context());
    }

    Exception::Exception(ExecutionContext* context, const std::string& message)
        : Object(context, context->get_native_objects().get_object_prototype()),
        _message(message) {}

    Exception::Exception(ExecutionContext* context, Object* parent, const std::string& message)
        : Object(context, parent),
        _message(message) {}

    std::string Exception::as_str() const {
        return _message;
    }

    const std::string& Exception::get_message() const {
        return _message;
    }

    Function::Function(ExecutionContext* context, std::shared_ptr<const Code> code, Module* globals)
        : Object(context, context->get_native_objects().get_object_prototype()),
        _code(code),
        _globals(globals) {}

    Function::Function(ExecutionContext* context, Object* parent, std::shared_ptr<const Code> code, Module* globals)
        : Object(context, parent),
        _code(code),
        _globals(globals) {}

    std::string Function::as_str() const {
        return fmt::format("<function {0}>", _code->get_label());
    }

    std::shared_ptr<const Code> Function::get_code() const {
        return _code;
    }

    Module* Function::get_globals() const {
        return _globals;
    }

    void Function::reach() {
        Object::reach();

        _globals->mark();
    }

    NativeFunction::NativeFunction(ExecutionContext* context, Callable callable)
        : Object(context, context->get_native_objects().get_object_prototype()),
        _callable(callable) {}

    NativeFunction::NativeFunction(ExecutionContext* context, Object* parent, Callable callable)
        : Object(context, parent),
        _callable(callable) {}
    
    std::string NativeFunction::as_str() const {
        return "<native_function>";
    }
    const NativeFunction::Callable& NativeFunction::get_callable() const {
        return _callable;
    }

    Object* NativeFunction::invoke(Object* receiver, NativeFrame* frame, ExecutionContext* context) {
        return _callable(receiver, frame, context);
    }

    Object* NativeFunction::operator()(Object* receiver, NativeFrame* frame, ExecutionContext* context) {
        return _callable(receiver, frame, context);
    }

    Null::Null(ExecutionContext* context)
        : Object(context) {}

    bool Null::as_bool() const {
        return false;
    }

    std::string Null::as_str() const {
        return "None";
    }

    const Object* Null::get_property(const std::string&) const {
        return nullptr;
    }

    const Object* Null::get_own_property(const std::string&) const {
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

    Number::Number(ExecutionContext* context, double value)
        : Object(context, context->get_native_objects().get_number_prototype()),
        _value(value) {}

    Number::Number(ExecutionContext* context, Object* parent, double value)
        : Object(context, parent),
        _value(value) {}

    bool Number::as_bool() const {
        return _value != 0;
    }

    std::string Number::as_str() const {
        return fmt::format("{0:g}", _value);
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

    String::String(ExecutionContext* context, const std::string& value)
        : Object(context, context->get_native_objects().get_string_prototype()),
        _value(value) {}

    String::String(ExecutionContext* context, Object* parent, const std::string& value)
        : Object(context, parent),
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
