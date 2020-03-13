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

#include "emerald/interpreter.h"
#include "emerald/magic_methods.h"
#include "emerald/module.h"
#include "emerald/object.h"
#include "emerald/objectutils.h"
#include "emerald/process.h"

namespace emerald {

    Object::Object(Process* process)
        : _process(process), 
        _parent(nullptr) {}

    Object::Object(Process* process, Object* parent)
        : _process(process), 
        _parent(parent) {}

    Object::~Object() {}

    bool Object::as_bool() const {
        return true;
    }
    
    std::string Object::as_str() const {
        return "<object>";
    }

    Process* Object::get_process() const {
        return _process;
    }

    Object* Object::get_parent() const {
        return _parent;
    }

    const std::unordered_map<std::string, PropertyDescriptor*>& Object::get_properties() const {
        return _properties;
    }

    Object* Object::get_property(const std::string& key) const {
        if (PropertyDescriptor* descriptor = get_property_descriptor(key)) {
            return get_property_value(descriptor);
        }

        return nullptr;
    }

    Object* Object::get_own_property(const std::string& key) const {
        if (PropertyDescriptor* descriptor = get_own_property_descriptor(key)) {
            return get_property_value(descriptor);
        }

        return nullptr;
    }

    PropertyDescriptor* Object::get_property_descriptor(const std::string& key) const {
        if (PropertyDescriptor* descriptor = get_own_property_descriptor(key)) {
            return descriptor;
        }

        if (_parent) {
            return _parent->get_property_descriptor(key);
        }

        return nullptr;
    }

    PropertyDescriptor* Object::get_own_property_descriptor(const std::string& key) const {
        if (has_own_property(key)) {
            return _properties.at(key);
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

    void Object::define_property(const std::string& key, PropertyDescriptor* descriptor) {
        _properties[key] = descriptor;
    }

    void Object::set_property(const std::string& key, Object* value) {
        if (PropertyDescriptor* descriptor = get_own_property_descriptor(key)) {
            if (descriptor->get_type() == PropertyDescriptor::DATA) {
                descriptor->set_value(value);
            } else if (Object* setter = descriptor->get_setter()) {
                Interpreter::call_obj<Object>(
                    setter,
                    this,
                    { value },
                    _process);
            }
        } else if (PropertyDescriptor* descriptor = get_property_descriptor(key);
                descriptor && descriptor->get_type() == PropertyDescriptor::ACCESSOR) {
            if (Object* setter = descriptor->get_setter()) {
                Interpreter::call_obj<Object>(
                    setter,
                    this,
                    { value },
                    _process);
            }
        } else {
            _properties[key] = _process->get_heap().allocate<PropertyDescriptor>(_process, value);
        }
    }

    Object* Object::clone(Process* process, CloneCache& cache) {
        return clone_impl<Object>(process, cache);
    } 

    void Object::reach() {
        if (_parent != nullptr) {
            _parent->mark();
        }

        for (std::pair<std::string, PropertyDescriptor*> pair : get_properties()) {
            pair.second->mark();
        }
    }

    Object* Object::get_property_value(PropertyDescriptor* descriptor) const {
        if (descriptor->get_type() == PropertyDescriptor::DATA) {
            return descriptor->get_value();
        }

        return Interpreter::call_obj<Object>(
            descriptor->get_getter(),
            const_cast<Object*>(this),
            {},
            _process);
    }

    Array::Array(Process* process, const std::vector<Object*>& value)
        : Object(process, ARRAY_PROTOTYPE),
        _value(value) {}

    Array::Array(Process* process, Object* parent, const std::vector<Object*>& value)
        : Object(process, parent),
        _value(value) {}

    bool Array::as_bool() const {
        return _value.size() > 0;
    }
    
    std::string Array::as_str() const {
        return "[" +
            objectutils::join_range(_value.begin(), _value.end(), ",", get_process())
        + "]";
    }

    void Array::init(Object* iterator) {
        objectutils::ObjectIterator iter = objectutils::ObjectIterator(get_process(), iterator);
        while (!iter.done()) {
            _value.push_back(iter.cur());
            iter.next();
        }
    }

    Object* Array::at(Number* n) const {
        size_t i = n->get_native_value();
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
        return BOOLEAN_IN_CTX(_value.empty(), get_process());
    }

    Number* Array::size() {
        return ALLOC_NUMBER_IN_CTX(_value.size(), get_process());
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

    String* Array::join(String* seperator) const {
        Process* process = get_process();
        return ALLOC_STRING(objectutils::join_range(
            _value.begin(),
            _value.end(),
            seperator->get_native_value(),
            process));
    }

    Boolean* Array::eq(Array* other) const {
        return BOOLEAN_IN_CTX(_eq(other), get_process());
    }

    Boolean* Array::neq(Array* other) const {
        return BOOLEAN_IN_CTX(!_eq(other), get_process());
    }

    Array* Array::clone(Process* process, CloneCache& cache) {
        Array* clone = clone_impl<Array>(process, cache, _value);
        for (Object* obj : _value) {
            clone->_value.push_back(obj->clone(process, cache));
        }
        return clone;
    }

    bool Array::_eq(Array* other) const {
        return objectutils::compare_range(
            _value.begin(),
            _value.end(),
            other->_value.begin(),
            get_process());
    }

    void Array::reach() {
        Object::reach();

        for (Object* obj : _value) {
            obj->mark();
        }
    }

    ArrayIterator::ArrayIterator(Process* process)
        : Object(process, ARRAY_ITERATOR_PROTOTYPE),
        _arr(nullptr),
        _i(0) {}

    ArrayIterator::ArrayIterator(Process* process, Object* parent)
        : Object(process, parent),
        _arr(nullptr),
        _i(0) {}

    std::string ArrayIterator::as_str() const {
        return "<array_iterator>";
    }

    void ArrayIterator::init(Array* arr) {
        _arr = arr;
    }

    Object* ArrayIterator::cur() const {
        if (_i >= _arr->_value.size()) {
            return _arr->back();
        }

        return _arr->_value[_i];
    }

    Boolean* ArrayIterator::done() const {
        return BOOLEAN_IN_CTX(_i == _arr->_value.size(), get_process());
    }

    Object* ArrayIterator::next() {
        _i++;
        return cur();
    }

    ArrayIterator* ArrayIterator::clone(Process* process, CloneCache& cache) {
        ArrayIterator* clone = clone_impl<ArrayIterator>(process, cache);
        clone->_arr = _arr->clone(process, cache);
        clone->_i = _i;
        return clone;
    }

    void ArrayIterator::reach() {
        Object::reach();

        if (_arr) {
            _arr->mark();
        }
    }

    Boolean::Boolean(Process* process, bool value)
        : Object(process, BOOLEAN_PROTOTYPE),
        _value(value) {}

    Boolean::Boolean(Process* process, Object* parent, bool value)
        : Object(process, parent),
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
        return BOOLEAN_IN_CTX(_value == other->_value, get_process());
    }

    Boolean* Boolean::neq(Boolean* other) const {
        return BOOLEAN_IN_CTX(_value != other->_value, get_process());
    }

    Boolean* Boolean::clone(Process* process, CloneCache& cache) {
        return clone_impl<Boolean>(process, cache, _value);
    }

    Exception::Exception(Process* process, const std::string& message)
        : Object(process, EXCEPTION_PROTOTYPE),
        _message(message) {}

    Exception::Exception(Process* process, Object* parent, const std::string& message)
        : Object(process, parent),
        _message(message) {}

    std::string Exception::as_str() const {
        return _message;
    }

    void Exception::init(String* message) {
        _message = message->get_native_value();
    }

    const std::string& Exception::get_message() const {
        return _message;
    }

    Exception* Exception::clone(Process* process, CloneCache& cache) {
        return clone_impl<Exception>(process, cache, _message);
    }

    Function::Function(Process* process, std::shared_ptr<const Code> code, Module* globals)
        : Object(process, OBJECT_PROTOTYPE),
        _code(code),
        _globals(globals) {}

    Function::Function(Process* process, Object* parent, std::shared_ptr<const Code> code, Module* globals)
        : Object(process, parent),
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

    Function* Function::clone(Process* process, CloneCache& cache) {
        return clone_impl<Function>(process, cache, _code, _globals->clone(process, cache));
    }

    NativeFunction::NativeFunction(Process* process, Callable callable, Module* globals)
        : Object(process, OBJECT_PROTOTYPE),
        _callable(callable),
        _globals(globals) {}

    NativeFunction::NativeFunction(Process* process, Object* parent, Callable callable, Module* globals)
        : Object(process, parent),
        _callable(callable),
        _globals(globals) {}
    
    std::string NativeFunction::as_str() const {
        return "<native_function>";
    }
    const NativeFunction::Callable& NativeFunction::get_callable() const {
        return _callable;
    }

    Module* NativeFunction::get_globals() const {
        return _globals;
    }

    Object* NativeFunction::invoke(Object* receiver, const std::vector<Object*>& args, Module* globals) {
        Process* process = get_process();
        NativeStack& native_stack = process->get_native_stack();
        NativeStack::NativeFrame frame = native_stack.push_frame(receiver, args, globals);
        Object* res = _callable(process, &frame);
        native_stack.pop_frame();
        return res;
    }

    Object* NativeFunction::operator()(Object* receiver, const std::vector<Object*>& args, Module* globals) {
        return invoke(receiver, args, globals);
    }

    NativeFunction* NativeFunction::clone(Process* process, CloneCache& cache) {
        Module* globals = nullptr;
        if (_globals) {
            globals = _globals->clone(process, cache);
        }
        return clone_impl<NativeFunction>(process, cache, _callable, globals);
    }

    Null::Null(Process* process)
        : Object(process, OBJECT_PROTOTYPE) {}

    Null::Null(Process* process, Object* parent)
        : Object(process, parent) {}

    bool Null::as_bool() const {
        return false;
    }

    std::string Null::as_str() const {
        return "None";
    }

    Null* Null::clone(Process* process, CloneCache& cache) {
        return clone_impl<Null>(process, cache);
    }

    Number::Number(Process* process, double value)
        : Object(process, NUMBER_PROTOTYPE),
        _value(value) {}

    Number::Number(Process* process, Object* parent, double value)
        : Object(process, parent),
        _value(value) {}

    bool Number::as_bool() const {
        return _value != 0;
    }

    std::string Number::as_str() const {
        std::string str = fmt::format("{0:f}", _value);
        str.erase(str.find_last_not_of('0') + 1);
        str.erase(str.find_last_not_of('.') + 1);
        return str;
    }

    void Number::init(Number* val) {
        _value = val->get_native_value();
    }

    double Number::get_native_value() const {
        return _value;
    }

    void Number::set_native_value(double val) {
        _value = val;
    }

    void Number::increment() {
        _value++;
    }

    void Number::decrement() {
        _value--;
    }

    Number* Number::clone(Process* process, CloneCache& cache) {
        return clone_impl<Number>(process, cache, _value);
    }

    PropertyDescriptor::PropertyDescriptor(Process* process, Object* value)
        : Object(process, OBJECT_PROTOTYPE),
        _type(DATA),
        _value(value) {}

    PropertyDescriptor::PropertyDescriptor(Process* process, Object* getter, Object* setter)
        : Object(process, OBJECT_PROTOTYPE),
        _type(ACCESSOR),
        _accessor(Accessor{.getter=getter, .setter=setter}) {}

    PropertyDescriptor::Type PropertyDescriptor::get_type() const {
        return _type;
    }

    Object* PropertyDescriptor::get_value() const {
        if (_type == DATA) {
            return _value;
        }

        return nullptr;
    }

    void PropertyDescriptor::set_value(Object* value) {
        if (_type == DATA) {
            _value = value;
        }
    }

    Object* PropertyDescriptor::get_getter() const {
        if (_type == ACCESSOR) {
            return _accessor.getter;
        }

        return nullptr;
    }

    Object* PropertyDescriptor::get_setter() const {
        if (_type == ACCESSOR) {
            return _accessor.setter;
        }

        return nullptr;
    }

    PropertyDescriptor* PropertyDescriptor::clone(Process* process, CloneCache& cache) {
        PropertyDescriptor* clone = clone_impl<PropertyDescriptor>(process, cache);
        if (_type == ACCESSOR) {
            clone->_accessor.getter = _accessor.getter->clone(process, cache);
            clone->_accessor.setter = _accessor.setter->clone(process, cache);
        } else {
            clone->_value = _value->clone(process, cache);
        }

        return clone;
    }

    void PropertyDescriptor::reach() {
        Object::reach();

        if (_type == ACCESSOR) {
            _accessor.getter->mark();
            _accessor.setter->mark();
        } else {
            _value->mark();
        }
    }

    String::String(Process* process, const std::string& value)
        : Object(process, STRING_PROTOTYPE),
        _value(value) {}

    String::String(Process* process, Object* parent, const std::string& value)
        : Object(process, parent),
        _value(value) {}

    bool String::as_bool() const {
        return _value.size() > 0;
    }

    std::string String::as_str() const {
        return _value;
    }

    void String::init(String* val) {
        _value = val->get_native_value();
    }

    std::string& String::get_native_value() {
        return _value;
    }

    const std::string& String::get_native_value() const {
        return _value;
    }

    String* String::clone(Process* process, CloneCache& cache) {
        return clone_impl<String>(process, cache, _value);
    }

    void CloneCache::add_clone(Object* obj, Object* clone) {
        _clones[obj] = clone;
    }

    Object* CloneCache::get_clone(Object* obj) {
        if (_clones.find(obj) != _clones.end()) {
            return _clones.at(obj);
        }

        return nullptr;
    }

    std::vector<HeapManaged*> CloneCache::get_roots() {
        std::vector<HeapManaged*> clones;
        for (const std::pair<Object*, Object*>& pair : _clones) {
            clones.push_back(pair.second);
        }

        return clones;
    }

} // namespace emerald
