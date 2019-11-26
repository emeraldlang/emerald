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

#ifndef _EMERALD_OBJECT_H
#define _EMERALD_OBJECT_H

#include <functional>
#include <string>
#include <utility>
#include <vector>

#include "emerald/code.h"
#include "emerald/heap.h"
#include "emerald/heap_managed.h"
#include "emerald/native_stack.h"
#include "emerald/process.h"

#define NATIVE_FUNCTION(name) Object* name(Process* process, NativeStack::NativeFrame* frame)

// Inheritance Hierarchy
// - Object
//      - Array
//      - Boolean
//      - Exception
//      - Function
//      - NativeFunction
//      - Null
//      - Number
//      - String

namespace emerald {

    class Boolean;
    class Number;
    class String;

    class CloneCache;
    class Module;

    class Object : public HeapManaged {
    public:
        Object(Process* process);
        Object(Process* process, Object* parent);
        virtual ~Object();

        virtual bool as_bool() const;
        virtual std::string as_str() const;

        Process* get_process() const;
        Object* get_parent() const;

        const std::unordered_map<std::string, Object*>& get_properties() const;

        const Object* get_property(const std::string& key) const;
        const Object* get_own_property(const std::string& key) const;
        Object* get_property(const std::string& key);
        Object* get_own_property(const std::string& key);

        bool has_property(const std::string& key) const;
        bool has_own_property(const std::string& key) const;

        bool set_property(const std::string& key, Object* value);

        virtual Object* clone(Process* process, CloneCache& cache);

    protected:
        template <class T, class... Args>
        T* clone_impl(Process* process, CloneCache& cache, Args&&... args);

        virtual void reach() override;

    private:
        Process* _process;
        Object* _parent;

        std::unordered_map<std::string, Object*> _properties;
    };

    class ArrayIterator;

    class Array final : public Object {
    public:
        Array(Process* process, const std::vector<Object*>& value = {});
        Array(Process* process, Object* parent, const std::vector<Object*>& value = {});

        bool as_bool() const override;
        std::string as_str() const override;

        void init(Object* iterator);

        Object* at(Number* n) const;
        Object* front() const;
        Object* back() const;

        Boolean* empty() const;
        Number* size();

        void clear();

        void push(Object* obj);
        Object* pop();

        String* join(String* seperator) const;

        Boolean* eq(Array* other) const;
        Boolean* neq(Array* other) const;

        Array* clone(Process* process, CloneCache& cache) override;

    private:
        friend class ArrayIterator;

        std::vector<Object*> _value;

        bool _eq(Array* other) const;

        void reach() override;
    };

    class ArrayIterator final : public Object {
    public:
        ArrayIterator(Process* process);
        ArrayIterator(Process* process, Object* parent);

        std::string as_str() const override;

        void init(Array* arr);

        Object* cur() const;
        Boolean* done() const;
        Object* next();

        ArrayIterator* clone(Process* process, CloneCache& cache) override;

    private:
        Array* _arr;
        size_t _i;

        void reach() override;
    };

    class Boolean final : public Object {
    public:
        Boolean(Process* process, bool value = false);
        Boolean(Process* process, Object* parent, bool value = false);

        bool as_bool() const override;
        std::string as_str() const override;

        void init(Boolean* val);

        bool get_native_value() const;

        Boolean* eq(Boolean* other) const;
        Boolean* neq(Boolean* other) const;

        Boolean* clone(Process* process, CloneCache& cache) override;

    private:
        bool _value;
    };

    class Exception : public Object {
    public:
        Exception(Process* process, const std::string& message = "");
        Exception(Process* process, Object* parent, const std::string& message = "");

        std::string as_str() const override;

        void init(String* message);

        const std::string& get_message() const;

        Exception* clone(Process* process, CloneCache& cache) override;

    private:
        std::string _message;
    };

    class Function final : public Object {
    public:
        Function(Process* process, std::shared_ptr<const Code> code, Module* globals);
        Function(Process* process, Object* parent, std::shared_ptr<const Code> code, Module* globals);

        std::string as_str() const override;

        std::shared_ptr<const Code> get_code() const;
        Module* get_globals() const;

        Function* clone(Process* process, CloneCache& cache) override;

    private:
        std::shared_ptr<const Code> _code;
        Module* _globals;

        void reach() override;
    };

    class NativeFunction final : public Object {
    public:
        using Callable = std::function<Object*(Process*, NativeStack::NativeFrame*)>;

        NativeFunction(Process* process, Callable callable, Module* globals = nullptr);
        NativeFunction(Process* process, Object* parent, Callable callable, Module* globals = nullptr);

        std::string as_str() const override;

        const Callable& get_callable() const;
        Module* get_globals() const;

        Object* invoke(Object* receiver, const std::vector<Object*>& args, Module* globals);
        Object* operator()(Object* receiver, const std::vector<Object*>& args, Module* globals);

        NativeFunction* clone(Process* process, CloneCache& cache) override;

    private:
        Callable _callable;
        Module* _globals;
    };

    class Null final : public Object {
    public:
        Null(Process* process);
        Null(Process* process, Object* parent);

        bool as_bool() const override;
        std::string as_str() const override;

        Null* clone(Process* process, CloneCache& cache) override;
    };

    class Number final : public Object {
    public:
        Number(Process* process, double value = 0);
        Number(Process* process, Object* parent, double value = 0);

        bool as_bool() const override;
        std::string as_str() const override;

        void init(Number* val);

        double get_native_value() const;
        void set_native_value(double val);

        void increment();
        void decrement();

        Number* clone(Process* process, CloneCache& cache) override;

    private:
        double _value;
    };

    class String final : public Object {
    public:
        String(Process* process, const std::string& value = "");
        String(Process* process, Object* parent, const std::string& value = "");

        bool as_bool() const override;
        std::string as_str() const override;

        void init(String* val);

        std::string& get_native_value();
        const std::string& get_native_value() const;

        String* clone(Process* process, CloneCache& cache) override;

    private:
        std::string _value;
    };

    class CloneCache final {
    public:
        CloneCache() = default;

        void add_clone(Object* obj, Object* clone);
        Object* get_clone(Object* obj);

    private:
        std::unordered_map<Object*, Object*> _clones;
    };

    template <class T, class... Args>
    T* Object::clone_impl(Process* process, CloneCache& cache, Args&&... args) {
        if (Object* obj = cache.get_clone(this)) {
            return static_cast<T*>(obj);
        }

        T* obj = process->get_heap().allocate<T>(
            process,
            nullptr,
            std::forward<Args>(args)...);
        cache.add_clone(this, obj);
        if (_parent) {
            // We have to clone the object first because
            // there may be a circular reference.
            obj->_parent = _parent->clone(process, cache);
        }
        for (std::pair<std::string, Object*> pair : get_properties()) {
            obj->set_property(pair.first, pair.second->clone(process, cache));
        }
        return obj;
    }

} // namespace emerald

#endif // _EMERALD_OBJECT_H
