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

#ifndef _EMERALD_NATIVE_FRAME_H
#define _EMERALD_NATIVE_FRAME_H

#include <string>
#include <unordered_map>
#include <vector>

#include "emerald/heap_root_source.h"
#include "emerald/object.h"

namespace emerald {

    class NativeFrame : public HeapRootSource {
    public:
        NativeFrame(const std::vector<Object*> args);

        const std::vector<Object*>& get_args() const;
        size_t num_args() const;

        const std::unordered_map<std::string, Object*>& get_locals() const;

        Object* get_arg(size_t arg_i);
        const Object* get_arg(size_t arg_i) const;

        Object* get_local(const std::string& name);
        const Object* get_local(const std::string& name) const;

        void set_local(const std::string& name, Object* val);

        Object*& operator[](const std::string& name);

        std::vector<HeapManaged*> get_roots() override;

    private:
        std::vector<Object*> _args;
        std::unordered_map<std::string, Object*> _locals; 
    };

} // namespace emerald

#endif // _EMERALD_NATIVE_FRAME_H
