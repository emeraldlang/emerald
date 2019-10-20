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

#define NATIVE_FUNCTION(name) Object* name(const std::vector<Object*>& args, ExecutionContext* context)

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

    class ExecutionContext;

    class Object : public HeapManaged {
    public:
        Object(ExecutionContext* context);
        Object(ExecutionContext* context, Object* parent);
        virtual ~Object();

        virtual bool as_bool() const;
        virtual std::string as_str() const;

        ExecutionContext* get_context() const;
        Object* get_parent() const;

        const std::unordered_map<std::string, Object*>& get_properties() const;

        virtual const Object* get_property(const std::string& key) const;
        virtual const Object* get_own_property(const std::string& key) const;
        Object* get_property(const std::string& key);
        Object* get_own_property(const std::string& key);

        virtual bool has_property(const std::string& key) const;
        virtual bool has_own_property(const std::string& key) const;

        virtual bool set_property(const std::string& key, Object* value);

    protected:
        virtual void reach() override;

    private:
        ExecutionContext* _context;
        Object* _parent;

        std::unordered_map<std::string, Object*> _properties;
    };

    class Array final : public Object {
    public:
        Array(ExecutionContext* context, const std::vector<Object*>& value = {});
        Array(ExecutionContext* context, Object* parent, const std::vector<Object*>& value = {});

        bool as_bool() const override;
        std::string as_str() const override;

        Object* at(size_t i) const;
        Object* front() const;
        Object* back() const;

        bool empty() const;
        size_t size();

        void clear();

        size_t push(Object* obj);
        Object* pop();

        bool operator==(const Array& other) const;
        bool operator!=(const Array& other) const;

    private:
        std::vector<Object*> _value;
    };

    class Boolean final : public Object {
    public:
        Boolean(ExecutionContext* context, bool value = false);
        Boolean(ExecutionContext* context, Object* parent, bool value = false);

        bool as_bool() const override;
        std::string as_str() const override;

        bool get_value() const;

    private:
        bool _value;
    };

    class Exception : public Object {
    public:
        Exception(ExecutionContext* context, const std::string& message);
        Exception(ExecutionContext* context, Object* parent, const std::string& message);

        std::string as_str() const override;

        const std::string& get_message() const;

    private:
        std::string _message;
    };

    class Function final : public Object {
    public:
        Function(ExecutionContext* context, std::shared_ptr<const Code> code);
        Function(ExecutionContext* context, Object* parent, std::shared_ptr<const Code> code);

        std::string as_str() const override;

        std::shared_ptr<const Code> get_code() const;

    private:
        std::shared_ptr<const Code> _code;
    };

    class NativeFunction final : public Object {
    public:
        using Callable = std::function<Object*(const std::vector<Object*>&, ExecutionContext*)>;

        NativeFunction(ExecutionContext* context, Callable callable);
        NativeFunction(ExecutionContext* context, Object* parent, Callable callable);

        std::string as_str() const override;

        const Callable& get_callable() const;

        Object* invoke(const std::vector<Object*>& args, ExecutionContext* context);
        Object* operator()(const std::vector<Object*>& args, ExecutionContext* context);

    private:
        Callable _callable;
    };

    class Null final : public Object {
    public:
        Null(ExecutionContext* context);

        bool as_bool() const override;
        std::string as_str() const override;

        const Object* get_property(const std::string& key) const override;
        const Object* get_own_property(const std::string& key) const override;

        bool has_property(const std::string& key) const override;
        bool has_own_property(const std::string& key) const override;

        bool set_property(const std::string& key, Object* value) override;
    };

    class Number final : public Object {
    public:
        Number(ExecutionContext* context, double value = 0);
        Number(ExecutionContext* context, Object* parent, double value = 0);

        bool as_bool() const override;
        std::string as_str() const override;

        double get_value() const;
        void set_value(double val);

        void increment();
        void decrement();

    private:
        double _value;
    };

    class String final : public Object {
    public:
        String(ExecutionContext* context, const std::string& value = "");
        String(ExecutionContext* context, Object* parent, const std::string& value = "");

        bool as_bool() const override;
        std::string as_str() const override;

        std::string& get_value();
        const std::string& get_value() const;

    private:
        std::string _value;
    };

} // namespace emerald

#endif // _EMERALD_OBJECT_H
