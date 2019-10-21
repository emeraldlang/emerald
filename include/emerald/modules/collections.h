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

#ifndef _EMERALD_MODULES_COLLECTIONS_H
#define _EMERALD_MODULES_COLLECTIONS_H

#include <deque>
#include <unordered_set>

#include "emerald/module_registry.h"
#include "emerald/object.h"

#define QUEUE_NATIVES   \
    X(queue_eq)         \
    X(queue_neq)        \
    X(queue_clone)      \
    X(queue_peek)       \
    X(queue_dequeue)    \
    X(queue_enqueue)    \
    X(queue_empty)      \
    X(queue_size)

#define SET_NATIVES \
    X(set_eq)       \
    X(set_neq)      \
    X(set_clone)    \
    X(set_add)      \
    X(set_contains) \
    X(set_remove)   \
    X(set_empty)    \
    X(set_size)

#define STACK_NATIVES   \
    X(stack_eq)         \
    X(stack_neq)        \
    X(stack_clone)      \
    X(stack_peek)       \
    X(stack_pop)        \
    X(stack_push)       \
    X(stack_empty)      \
    X(stack_size)

namespace emerald {
namespace modules {

    class Queue : public Object {
    public:
        Queue(ExecutionContext* context);
        Queue(ExecutionContext* context, Object* parent);

        std::string as_str() const override;

        Object* peek();
        Object* dequeue();
        size_t enqueue(Object* obj);

        bool empty() const;
        size_t size() const;

        bool operator==(const Queue& other) const;
        bool operator!=(const Queue& other) const;

    private:
        std::deque<Object*> _value;
    };

    class Set : public Object {
    public:
        Set(ExecutionContext* context);
        Set(ExecutionContext* context, Object* parent);

        std::string as_str() const override;

        size_t add(Object* obj);
        bool contains(Object* obj) const;
        size_t remove(Object* obj);

        bool empty() const;
        size_t size() const;

        bool operator==(const Set& other) const;
        bool operator!=(const Set& other) const;

    private:
        struct hash {
            size_t operator()(Object* val) const;
        };

        struct key_eq {
            bool operator()(Object* lhs, Object* rhs) const;
        };

        std::unordered_set<Object*, hash, key_eq> _value;
    };

    class Stack : public Object {
    public:
        Stack(ExecutionContext* context);
        Stack(ExecutionContext* context, Object* parent);

        std::string as_str() const override;

        Object* peek() const;
        Object* pop();
        size_t push(Object* obj);

        bool empty() const;
        size_t size() const;

        bool operator==(const Stack& other) const;
        bool operator!=(const Stack& other) const;

    private:
        std::deque<Object*> _value;
    };

#define X(name) NATIVE_FUNCTION(name);
    QUEUE_NATIVES
    SET_NATIVES
    STACK_NATIVES
#undef X

    MODULE_INITIALIZATION_FUNC(init_collections_module);

} // namespace modules
} // namespace emerald

#endif // _EMERALD_MODULES_COLLECTIONS_H
