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

#ifndef _EMERALD_MODULE_H
#define _EMERALD_MODULE_H

#include <filesystem>
#include <string>

#include "emerald/code.h"
#include "emerald/heap.h"
#include "emerald/object.h"

namespace emerald {

    class Module : public Object {
    public:
        Module(ExecutionContext* context, const std::string& name);
        Module(ExecutionContext* context, const std::string& name, std::shared_ptr<Code> code);

        const std::string& get_name() const;
        std::shared_ptr<Code> get_code() const;

        bool is_native() const;

        std::string as_str() const override;

        static std::filesystem::path get_module_path(
            const std::string& module_name,
            const std::string& extension);

        static std::filesystem::path get_stdlib_path();

    private:
        std::string _name;
        std::shared_ptr<Code> _code;
    };

} // namespace emerald

#endif // _EMERALD_MODULE_H
