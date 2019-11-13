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

#ifndef _EMERALD_MODULES_IO_H
#define _EMERALD_MODULES_IO_H

#include <fstream>
#include <sstream>
#include <string>

#include "emerald/module_registry.h"
#include "emerald/object.h"

#define FILE_STREAM_NATIVES \
    X(file_stream_clone)    \
    X(file_stream_open)     \
    X(file_stream_is_open)  \
    X(file_stream_read)     \
    X(file_stream_readline) \
    X(file_stream_write)

#define STRING_STREAM_NATIVES   \
    X(string_stream_clone)      \
    X(string_stream_read)       \
    X(string_stream_readline)   \
    X(string_stream_write)

namespace emerald {
namespace modules {

    class FileStream final : public Object {
    public:
        FileStream(Process* process);
        FileStream(Process* process, Object* parent);

        std::string as_str() const override;

        void open(String* filename, String* access);
        Boolean* is_open() const;

        String* read();
        String* read(Number* n);
        String* readline();
        void write(String* s);

        FileStream* clone(Process* process, CloneCache& cache) override;

    private:
        std::fstream _stream;
    };

    class StringStream final : public Object {
    public:
        StringStream(Process* process);
        StringStream(Process* process, Object* parent);

        std::string as_str() const override;

        String* read(Number* n);
        String* readline();
        void write(String* s);

        StringStream* clone(Process* process, CloneCache& cache) override;

    private:
        std::stringstream _stream;
    };

#define X(name) NATIVE_FUNCTION(name);
    FILE_STREAM_NATIVES
    STRING_STREAM_NATIVES
#undef X

    MODULE_INITIALIZATION_FUNC(init_io_module);

} // namespace modules
} // namespace emerald

#endif // _EMERALD_MODULES_IO_H
