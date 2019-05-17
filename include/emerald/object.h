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
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "emerald/code.h"
#include "emerald/heap.h"
#include "emerald/heap_managed.h"

#define NATIVE_FUNCTION(name) Object* name(Heap* heap, const std::vector<Object*>& args)

// Inheritance Hierarchy
// - Object
//      - NativeFunction
//      - HeapObject
//          - Array
//          - Boolean
//          - Exception
//          - Function
//          - Number
//          - String

namespace emerald {

    class Object {
    public:
        Object();
        Object(Object* parent);
        virtual ~Object();

        virtual bool as_bool() const;
        virtual std::string as_str() const = 0;

        Object* get_parent() const;

        const std::unordered_map<std::string, Object*>& get_properties() const;

        Object* get_property(const std::string& key);
        Object* get_own_property(const std::string& key);

        bool has_property(const std::string& key) const;
        bool has_own_property(const std::string& key) const;

        void set_property(const std::string& key, Object* value);

    private:
        Object* _parent;
        std::unordered_map<std::string, Object*> _properties;
    };

    class NativeFunction final : public Object {
    public:
        using Callable = std::function<Object*(Heap*, const std::vector<Object*>&)>;

        NativeFunction(Callable callable);
        NativeFunction(Object* parent, Callable callable);

        std::string as_str() const override;

        const Callable& get_callable() const;

        Object* invoke(Heap* heap, const std::vector<Object*>& args);
        Object* operator()(Heap* heap, const std::vector<Object*>& args);

    private:
        Callable _callable;
    };

    class HeapObject : public Object, public HeapManaged {
    public:
        HeapObject(Heap* heap);
        HeapObject(Heap* heap, Object* parent);

    protected:
        virtual void reach() override;
    };

    class Array final : public HeapObject {
    public:
        Array(Heap* heap);
        Array(Heap* heap, Object* parent);

        bool as_bool() const override;
        std::string as_str() const override;

        std::vector<Object*>& get_value();
        const std::vector<Object*>& get_value() const;

    private:
        std::vector<Object*> _value;
    };

    class Boolean final : public HeapObject {
    public:
        Boolean(Heap* heap, bool value);
        Boolean(Heap* heap, Object* parent, bool value);

        bool as_bool() const override;
        std::string as_str() const override;

        bool get_value() const;

    private:
        bool _value;
    };

    class Exception : public HeapObject {
    public:
        Exception(Heap* heap, const std::string& message);
        Exception(Heap* heap, Object* parent, const std::string& message);

        std::string as_str() const override;

        const std::string& get_message() const;

    private:
        std::string _message;
    };

    class Function final : public HeapObject {
    public:
        Function(Heap* heap, std::shared_ptr<const Code> code);
        Function(Heap* heap, Object* parent, std::shared_ptr<const Code> code);

        std::string as_str() const override;

        std::shared_ptr<const Code> get_code() const;

    private:
        std::shared_ptr<const Code> _code;
    };

    class Number final : public HeapObject {
    public:
        Number(Heap* heap, double value);
        Number(Heap* heap, Object* parent, double value);

        bool as_bool() const override;
        std::string as_str() const override;

        double get_value() const;
        void set_value(double val);

        void increment();
        void decrement();

    private:
        double _value;

        void initialize_properties();
    };

    class String final : public HeapObject {
    public:
        String(Heap* heap, const std::string& value);
        String(Heap* heap, Object* parent, const std::string& value);

        bool as_bool() const override;
        std::string as_str() const override;

        std::string& get_value();
        const std::string& get_value() const;

    private:
        std::string _value;

        void initialize_properties();
    };

} // namespace emerald

#endif // _EMERALD_OBJECT_H
