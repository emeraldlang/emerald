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

#include "emerald/native_frame.h"

namespace emerald {

    NativeFrame::NativeFrame(const std::vector<Object*> args)
        : _args(args) {}

    const std::vector<Object*>& NativeFrame::get_args() const {
        return _args;
    }

    size_t NativeFrame::num_args() const {
        return _args.size();
    }

    const std::unordered_map<std::string, Object*>& NativeFrame::get_locals() const {
        return _locals;
    }

    Object* NativeFrame::get_arg(size_t i) {
        return _args.at(i);
    }

    const Object* NativeFrame::get_arg(size_t i) const {
        return _args.at(i);
    }

    Object* NativeFrame::get_local(const std::string& name) {
        return _locals.at(name);
    }

    const Object* NativeFrame::get_local(const std::string& name) const {
        return _locals.at(name);
    }

    void NativeFrame::set_local(const std::string& name, Object* val) {
        _locals[name] = val;
    }

    Object*& NativeFrame::operator[](const std::string& name) {
        return _locals[name];
    }

    std::vector<HeapManaged*> NativeFrame::get_roots() {
        std::vector<HeapManaged*> roots;

        for (Object* arg : _args) {
            roots.push_back(arg);
        }

        for (std::pair<std::string, Object*> local : _locals) {
            roots.push_back(local.second);
        }

        return roots;
    }

} // namespace emerald
