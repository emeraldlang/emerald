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

#include "fmt/format.h"

#include "emerald/magic_methods.h"
#include "emerald/module.h"
#include "emerald/modules/io.h"
#include "emerald/native_variables.h"
#include "emerald/objectutils.h"
#include "emerald/process.h"

namespace emerald {
namespace modules {

    FileStream::FileStream(Process* process)
        : Object(process, OBJECT_PROTOTYPE) {}

    FileStream::FileStream(Process* process, Object* parent)
        : Object(process, parent) {}

    std::string FileStream::as_str() const {
        return "<file_stream>";
    }

    void FileStream::open(String* filename, String* access) {
        std::fstream::openmode openmode;
        const std::string& access_str = access->get_native_value();
        if (access_str == "read") {
            openmode = std::fstream::in;
        } else if (access_str == "write") {
            openmode = std::fstream::out;
        } else if (access_str == "read_write") {
            openmode = std::fstream::in | std::fstream::out;
        } else {
            std::string msg = fmt::format("unknown file access: {0}", access_str);
            throw ALLOC_EXCEPTION_IN_CTX(msg, get_process());
        }

        _stream.open(filename->get_native_value(), openmode);
    }

    Boolean* FileStream::is_open() const {
        return BOOLEAN_IN_CTX(_stream.is_open(), get_process());
    }

    String* FileStream::read() {
        size_t cp = _stream.tellg();
        _stream.seekg(0, _stream.end);
        size_t size = _stream.tellg();
        _stream.seekg(cp);

        char s[size];
        _stream.read(s, size);

        return ALLOC_STRING_IN_CTX(std::string(s), get_process());
    }

    String* FileStream::read(Number* n) {
        size_t size = n->get_native_value(); 
        char s[size];
        _stream.read(s, size);
        return ALLOC_STRING_IN_CTX(std::string(s), get_process());
    }

    String* FileStream::readline() {
        std::string s;
        std::getline(_stream, s);
        return ALLOC_STRING_IN_CTX(s, get_process());
    }

    void FileStream::write(String* s) {
        const std::string& snv = s->get_native_value();
        _stream.write(snv.data(), snv.size());
    }

    FileStream* FileStream::clone(Process* process, CloneCache& cache) {
        return clone_impl<FileStream>(process, cache);
    }

    StringStream::StringStream(Process* process)
        : Object(process, OBJECT_PROTOTYPE) {}

    StringStream::StringStream(Process* process, Object* parent)
        : Object(process, parent) {}

    std::string StringStream::as_str() const {
        return "<string_stream>";
    }

    String* StringStream::read(Number* n) {
        size_t size = n->get_native_value();
        char s[size];
        _stream.read(s, size);
        return ALLOC_STRING_IN_CTX(std::string(s), get_process());
    }

    String* StringStream::readline() {
        std::string s;
        std::getline(_stream, s);
        return ALLOC_STRING_IN_CTX(s, get_process());
    }

    void StringStream::write(String* s) {
        const std::string& snv = s->get_native_value();
        _stream.write(snv.data(), snv.size());
    }

    StringStream* StringStream::clone(Process* process, CloneCache& cache) {
        return clone_impl<StringStream>(process, cache);
    }

    NATIVE_FUNCTION(file_stream_clone) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(FileStream, self); 
        return process->get_heap().allocate<FileStream>(process, self);
    }

    NATIVE_FUNCTION(file_stream_open) {
        EXPECT_NUM_ARGS(2);

        CONVERT_RECV_TO(FileStream, self);
        CONVERT_ARG_TO(0, String, filename);
        CONVERT_ARG_TO(1, String, access);

        self->open(filename, access);

        return self->is_open();
    }
    
    NATIVE_FUNCTION(file_stream_is_open) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(FileStream, self);

        return self->is_open();
    }

    NATIVE_FUNCTION(file_stream_read) {
        CONVERT_RECV_TO(FileStream, self);

        TRY_CONVERT_OPTIONAL_ARG_TO(0, Number, count);
        if (count) {
            return self->read(count);
        }

        return self->read();
    }

    NATIVE_FUNCTION(file_stream_readline) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(FileStream, self);

        return self->readline();
    }

    NATIVE_FUNCTION(file_stream_write) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(FileStream, self);
        CONVERT_ARG_TO(0, String, s);

        self->write(s);

        return NONE;
    }

    NATIVE_FUNCTION(string_stream_clone) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(StringStream, self);

        return process->get_heap().allocate<StringStream>(process, self);
    }

    NATIVE_FUNCTION(string_stream_read) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(StringStream, self);
        CONVERT_ARG_TO(0, Number, count);

        return self->read(count);
    }

    NATIVE_FUNCTION(string_stream_readline) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(StringStream, self);

        return self->readline();
    }

    NATIVE_FUNCTION(string_stream_write) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(StringStream, self);
        CONVERT_ARG_TO(0, String, s);

        self->write(s);

        return NONE;
    }

    MODULE_INITIALIZATION_FUNC(init_io_module) {
        Process* process =  module->get_process();

        Local<FileStream> file_stream = process->get_heap().allocate<FileStream>(process);
        file_stream->set_property(magic_methods::clone, ALLOC_NATIVE_FUNCTION(file_stream_clone));
        file_stream->set_property("open", ALLOC_NATIVE_FUNCTION(file_stream_open));
        file_stream->set_property("is_open", ALLOC_NATIVE_FUNCTION(file_stream_is_open));
        file_stream->set_property("read", ALLOC_NATIVE_FUNCTION(file_stream_read));
        file_stream->set_property("readline", ALLOC_NATIVE_FUNCTION(file_stream_readline));
        file_stream->set_property("write", ALLOC_NATIVE_FUNCTION(file_stream_write));
        module->set_property("FileStream", file_stream.val());

        Local<Object> file_access = ALLOC_OBJECT();
        file_access->set_property("read", ALLOC_STRING("read"));
        file_access->set_property("write", ALLOC_STRING("write"));
        file_access->set_property("read_write", ALLOC_STRING("read_write"));
        module->set_property("FileAccess", file_access.val());

        Local<StringStream> string_stream = process->get_heap().allocate<StringStream>(process);
        string_stream->set_property(magic_methods::clone, ALLOC_NATIVE_FUNCTION(string_stream_clone));
        string_stream->set_property("read", ALLOC_NATIVE_FUNCTION(string_stream_read));
        string_stream->set_property("readline", ALLOC_NATIVE_FUNCTION(string_stream_readline));
        string_stream->set_property("write", ALLOC_NATIVE_FUNCTION(string_stream_write));
        module->set_property("StringStream", string_stream.val());
    }

} // namespace modules
} // namespace emerald
