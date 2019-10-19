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

#include <functional>

#include "emerald/execution_context.h"
#include "emerald/interpreter.h"
#include "emerald/magic_methods.h"
#include "emerald/module.h"
#include "emerald/modules/collections.h"
#include "emerald/objectutils.h"
#include "emerald/strutils.h"

namespace emerald {
namespace modules {

    Queue::Queue(ExecutionContext* context)
        : Object(context) {}

    Queue::Queue(ExecutionContext* context, Object* parent)
        : Object(context, parent) {}

    std::string Queue::as_str() const {
        return "queue(" + 
            objectutils::range_to_str(_value.begin(), _value.end(), get_context()) 
        + ")";
    }

    Object* Queue::peek() {
        return _value.front();
    }

    Object* Queue::dequeue() {
        Object* obj = _value.front();
        _value.pop_front();
        return obj;
    }

    size_t Queue::enqueue(Object* obj) {
        _value.push_back(obj);
        return _value.size();
    }

    bool Queue::empty() const {
        return _value.empty();
    }

    size_t Queue::size() const {
        return _value.size();
    }

    bool Queue::operator==(const Queue& other) const {
        return objectutils::compare_range(
            _value.begin(),
            _value.end(),
            other._value.begin(),
            get_context());
    }

    bool Queue::operator!=(const Queue& other) const {
        return !(*this == other);
    }

    Set::Set(ExecutionContext* context)
        : Object(context) {}
    
    Set::Set(ExecutionContext* context, Object* parent)
        : Object(context, parent) {}

    std::string Set::as_str() const {
        return "set(" + 
            objectutils::range_to_str(_value.begin(), _value.end(), get_context()) 
        + ")";
    }

    size_t Set::add(Object* obj) {
        _value.insert(obj);
        return _value.size();
    }

    bool Set::contains(Object* obj) const {
        return _value.find(obj) != _value.end();
    }

    size_t Set::remove(Object* obj) {
        _value.erase(obj);
        return _value.size();
    }

    bool Set::empty() const {
        return _value.empty();
    }

    size_t Set::size() const {
        return _value.size();
    }

    bool Set::operator==(const Set& other) const {
        return _value == other._value;
    }

    bool Set::operator!=(const Set& other) const {
        return !(*this == other);
    }

    size_t Set::hash::operator()(Object* val) const {
        return std::hash<std::string>{}(
            Interpreter::execute_method(
                magic_methods::str,
                { val },
                val->get_context())->as_str());
    }

    bool Set::key_eq::operator()(Object* lhs, Object* rhs) const {
        return Interpreter::execute_method(
            magic_methods::eq,
            { lhs, rhs },
            lhs->get_context())->as_bool();
    }

    Stack::Stack(ExecutionContext* context)
        : Object(context) {}

    Stack::Stack(ExecutionContext* context, Object* parent)
        : Object(context, parent) {}

    std::string Stack::as_str() const {
        return "stack(" + 
            objectutils::range_to_str(_value.begin(), _value.end(), get_context()) 
        + ")";
    }

    Object* Stack::peek() const {
        return _value.back();
    }

    Object* Stack::pop() {
        Object* top = _value.back();
        _value.pop_back();
        return top;
    }

    size_t Stack::push(Object* obj) {
        _value.push_back(obj);
        return _value.size();
    }

    bool Stack::empty() const {
        return _value.empty();
    }

    size_t Stack::size() const {
        return _value.size();
    }

    bool Stack::operator==(const Stack& other) const {
        return objectutils::compare_range(
            _value.begin(),
            _value.end(),
            other._value.begin(),
            get_context());
    }

    bool Stack::operator!=(const Stack& other) const {
        return !(*this == other);
    }

    NATIVE_FUNCTION(queue_eq) {
        EXPECT_NUM_ARGS(2);

        CONVERT_RECV_TO(Queue, self);

        if (TRY_CONVERT_ARG_TO(1, Queue, other)) {
            return BOOLEAN(*self == *other);
        }

        return BOOLEAN(false);
    }

    NATIVE_FUNCTION(queue_neq) {
        EXPECT_NUM_ARGS(2);

        CONVERT_RECV_TO(Queue, self);

        if (TRY_CONVERT_ARG_TO(1, Queue, other)) {
            return BOOLEAN(*self != *other);
        }

        return BOOLEAN(true);
    }

    NATIVE_FUNCTION(queue_clone) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(Queue, self);

        return context->get_heap().allocate<Queue>(context, self);
    }

    NATIVE_FUNCTION(queue_peek) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(Queue, self);

        return self->peek();
    }

    NATIVE_FUNCTION(queue_dequeue) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(Queue, self);

        return self->dequeue();
    }

    NATIVE_FUNCTION(queue_enqueue) {
        EXPECT_ATLEAST_NUM_ARGS(1);

        CONVERT_RECV_TO(Queue, self);

        size_t n = args.size();
        for (size_t i = 1; i < n; i++) {
            self->enqueue(args[i]);
        }

        return ALLOC_NUMBER(self->size());
    }

    NATIVE_FUNCTION(queue_empty) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(Queue, self);

        return BOOLEAN(self->empty());
    }

    NATIVE_FUNCTION(queue_size) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(Queue, self);

        return ALLOC_NUMBER(self->size());
    }

    NATIVE_FUNCTION(set_eq) {
        EXPECT_NUM_ARGS(2);

        CONVERT_RECV_TO(Set, self);

        if (TRY_CONVERT_ARG_TO(1, Set, other)) {
            return BOOLEAN(*self == *other);
        }

        return BOOLEAN(false);
    }

    NATIVE_FUNCTION(set_neq) {
        EXPECT_NUM_ARGS(2);

        CONVERT_RECV_TO(Set, self);

        if (TRY_CONVERT_ARG_TO(1, Set, other)) {
            return BOOLEAN(*self != *other);
        }

        return BOOLEAN(true);
    }

    NATIVE_FUNCTION(set_clone) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(Set, self);

        return context->get_heap().allocate<Set>(context, self);
    }

    NATIVE_FUNCTION(set_add) {
        EXPECT_ATLEAST_NUM_ARGS(1);

        CONVERT_RECV_TO(Set, self);

        size_t n = args.size();
        for (size_t i = 1; i < n; i++) {
            self->add(args[i]);
        }

        return ALLOC_NUMBER(self->size());
    }

    NATIVE_FUNCTION(set_contains) {
        EXPECT_NUM_ARGS(2);

        CONVERT_RECV_TO(Set, self);

        return BOOLEAN(self->contains(args[1]));
    }

    NATIVE_FUNCTION(set_remove) {
        EXPECT_NUM_ARGS(2);

        CONVERT_RECV_TO(Set, self);

        return ALLOC_NUMBER(self->remove(args[1]));
    }

    NATIVE_FUNCTION(set_empty) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(Set, self);

        return BOOLEAN(self->empty());
    }

    NATIVE_FUNCTION(set_size) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(Set, self);

        return ALLOC_NUMBER(self->size());
    }

    NATIVE_FUNCTION(stack_eq) {
        EXPECT_NUM_ARGS(2);

        CONVERT_RECV_TO(Stack, self);

        if (TRY_CONVERT_ARG_TO(1, Stack, other)) {
            return BOOLEAN(*self == *other);
        }

        return BOOLEAN(false);
    }

    NATIVE_FUNCTION(stack_neq) {
        EXPECT_NUM_ARGS(2);

        CONVERT_RECV_TO(Stack, self);

        if (TRY_CONVERT_ARG_TO(1, Stack, other)) {
            return BOOLEAN(*self != *other);
        }

        return BOOLEAN(true);
    }

    NATIVE_FUNCTION(stack_clone) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(Stack, self);

        return context->get_heap().allocate<Stack>(context, self);
    }

    NATIVE_FUNCTION(stack_peek) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(Stack, self);

        return self->peek();
    }

    NATIVE_FUNCTION(stack_pop) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(Stack, self);

        return self->pop();
    }

    NATIVE_FUNCTION(stack_push) {
        EXPECT_ATLEAST_NUM_ARGS(1);

        CONVERT_RECV_TO(Stack, self);

        size_t n = args.size();
        for (size_t i = 1; i < n; i++) {
            self->push(args[i]);
        }

        return ALLOC_NUMBER(self->size());
    }

    NATIVE_FUNCTION(stack_empty) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(Stack, self);

        return BOOLEAN(self->empty());
    }

    NATIVE_FUNCTION(stack_size) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(Stack, self);

        return ALLOC_NUMBER(self->size());
    }

    MODULE_INITIALIZATION_FUNC(init_collections_module) {
        Heap& heap = context->get_heap();
        NativeObjects& native_objects = context->get_native_objects();

        Module* module = heap.allocate<Module>(context, "collections");

        Queue* queue = heap.allocate<Queue>(context, native_objects.get_object_prototype());
        queue->set_property(magic_methods::eq, ALLOC_NATIVE_FUNCTION(queue_eq));
        queue->set_property(magic_methods::neq, ALLOC_NATIVE_FUNCTION(queue_neq));
        queue->set_property(magic_methods::clone, ALLOC_NATIVE_FUNCTION(queue_clone));
        queue->set_property("peek", ALLOC_NATIVE_FUNCTION(queue_peek));
        queue->set_property("dequeue", ALLOC_NATIVE_FUNCTION(queue_dequeue));
        queue->set_property("enqueue", ALLOC_NATIVE_FUNCTION(queue_enqueue));
        queue->set_property("empty", ALLOC_NATIVE_FUNCTION(queue_empty));
        queue->set_property("size", ALLOC_NATIVE_FUNCTION(queue_size));
        module->set_property("Queue", queue);

        Set* set = heap.allocate<Set>(context, native_objects.get_object_prototype());
        set->set_property(magic_methods::eq, ALLOC_NATIVE_FUNCTION(set_eq));
        set->set_property(magic_methods::neq, ALLOC_NATIVE_FUNCTION(set_neq));
        set->set_property(magic_methods::clone, ALLOC_NATIVE_FUNCTION(set_clone));
        set->set_property("add", ALLOC_NATIVE_FUNCTION(set_add));
        set->set_property("contains", ALLOC_NATIVE_FUNCTION(set_contains));
        set->set_property("remove", ALLOC_NATIVE_FUNCTION(set_remove));
        set->set_property("empty", ALLOC_NATIVE_FUNCTION(set_empty));
        set->set_property("size", ALLOC_NATIVE_FUNCTION(set_size));
        module->set_property("Set", set);

        Stack* stack = heap.allocate<Stack>(context, native_objects.get_object_prototype());
        stack->set_property(magic_methods::eq, ALLOC_NATIVE_FUNCTION(stack_eq));
        stack->set_property(magic_methods::neq, ALLOC_NATIVE_FUNCTION(stack_neq));
        stack->set_property(magic_methods::clone, ALLOC_NATIVE_FUNCTION(stack_clone));
        stack->set_property("peek", ALLOC_NATIVE_FUNCTION(stack_peek));
        stack->set_property("pop", ALLOC_NATIVE_FUNCTION(stack_pop));
        stack->set_property("push", ALLOC_NATIVE_FUNCTION(stack_push));
        stack->set_property("empty", ALLOC_NATIVE_FUNCTION(stack_empty));
        stack->set_property("size", ALLOC_NATIVE_FUNCTION(stack_size));
        module->set_property("Stack", stack);

        return module;
    }

} // namespace modules
} // namespace emerald
