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

#include "emerald/module.h"
#include "emerald/native_objects.h"
#include "emerald/object.h"

#define FILE_STREAM_NATIVES \
    X(file_stream_clone)    \
    X(file_stream_open)     \
    X(file_stream_is_open)  \
    X(file_stream_read)     \
    X(file_stream_write)

#define STRING_STREAM_NATIVES   \
    X(string_stream_clone)      \
    X(string_stream_read)       \
    X(string_stream_write)

namespace emerald {
namespace modules {

    class FileStream final : public HeapObject {
    public:
        FileStream();
        FileStream(Object* parent);

        std::string as_str() const override;

        void open(const std::string& filename, std::fstream::openmode mode);
        bool is_open() const;

        std::string read();
        std::string read(size_t n);
        void write(const std::string& s);

    private:
        std::fstream _stream;
    };

    class StringStream final : public HeapObject {
    public:
        StringStream();
        StringStream(Object* parent);

        std::string as_str() const override;

        std::string read(size_t n);
        void write(const std::string& s);

    private:
        std::stringstream _stream;
    };

#define X(name) NativeFunction* get_##name();
    FILE_STREAM_NATIVES
    STRING_STREAM_NATIVES
#undef X

#define X(name) NATIVE_FUNCTION(name);
    FILE_STREAM_NATIVES
    STRING_STREAM_NATIVES
#undef X

    MODULE_INITIALIZATION_FUNC(init_io_module);

} // namespace modules
} // namespace emerald

#endif // _EMERALD_MODULES_IO_H
