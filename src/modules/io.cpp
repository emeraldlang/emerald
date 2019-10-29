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

#include "emerald/execution_context.h"
#include "emerald/magic_methods.h"
#include "emerald/module.h"
#include "emerald/modules/io.h"
#include "emerald/native_frame.h"
#include "emerald/objectutils.h"

namespace emerald {
namespace modules {

    FileStream::FileStream(ExecutionContext* context)
        : Object(context) {}

    FileStream::FileStream(ExecutionContext* context, Object* parent)
        : Object(context, parent) {}

    std::string FileStream::as_str() const {
        return "<file_stream>";
    }

    void FileStream::open(const std::string& filename, std::fstream::openmode mode) {
        _stream.open(filename, mode);
    }

    bool FileStream::is_open() const {
        return _stream.is_open();
    }

    std::string FileStream::read() {
        size_t cp = _stream.tellg();
        _stream.seekg(0, _stream.end);
        size_t size = _stream.tellg();
        _stream.seekg(cp);

        return read(size);
    }

    std::string FileStream::read(size_t n) {
        char s[n];
        _stream.read(s, n);
        return std::string(s);
    }

    std::string FileStream::readline() {
        std::string s;
        std::getline(_stream, s);
        return s;
    }

    void FileStream::write(const std::string& s) {
        _stream.write(s.data(), s.size());
    }

    StringStream::StringStream(ExecutionContext* context)
        : Object(context) {}

    StringStream::StringStream(ExecutionContext* context, Object* parent)
        : Object(context, parent) {}

    std::string StringStream::as_str() const {
        return "<string_stream>";
    }

    std::string StringStream::read(size_t n) {
        char s[n];
        _stream.read(s, n);
        return std::string(s);
    }

    std::string StringStream::readline() {
        std::string s;
        std::getline(_stream, s);
        return s;
    }

    void StringStream::write(const std::string& s) {
        _stream.write(s.data(), s.size());
    }

    NATIVE_FUNCTION(file_stream_clone) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(FileStream, self); 
        return context->get_heap().allocate<FileStream>(context, self);
    }

    NATIVE_FUNCTION(file_stream_open) {
        EXPECT_NUM_ARGS(2);

        CONVERT_RECV_TO(FileStream, stream);
        CONVERT_ARG_TO(0, String, filename);
        CONVERT_ARG_TO(1, String, access);

        std::fstream::openmode openmode;
        const std::string& access_str = access->get_value();
        if (access_str == "read") {
            openmode = std::fstream::in;
        } else if (access_str == "write") {
            openmode = std::fstream::out;
        } else if (access_str == "read_write") {
            openmode = std::fstream::in | std::fstream::out;
        } else {
            throw context->get_heap().allocate<Exception>(
                context, fmt::format("unknown file access: {0}", access_str));
        }

        stream->open(filename->get_value(), openmode);

        return BOOLEAN(stream->is_open());
    }
    
    NATIVE_FUNCTION(file_stream_is_open) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(FileStream, stream);

        return BOOLEAN(stream->is_open());
    }

    NATIVE_FUNCTION(file_stream_read) {
        CONVERT_RECV_TO(FileStream, stream);

        TRY_CONVERT_OPTIONAL_ARG_TO(0, Number, count);
        std::string result;
        if (count) {
            result = stream->read((long)count->get_value());
        } else {
            result = stream->read();
        }

        return ALLOC_STRING(result);
    }

    NATIVE_FUNCTION(file_stream_readline) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(FileStream, stream);

        return ALLOC_STRING(stream->readline());
    }

    NATIVE_FUNCTION(file_stream_write) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(FileStream, stream);
        CONVERT_ARG_TO(0, String, s);

        stream->write(s->get_value());

        return ALLOC_NUMBER(s->get_value().length());
    }

    NATIVE_FUNCTION(string_stream_clone) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(StringStream, self);

        return context->get_heap().allocate<StringStream>(context, self);
    }

    NATIVE_FUNCTION(string_stream_read) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(StringStream, stream);
        CONVERT_ARG_TO(0, Number, count);

        return ALLOC_STRING(stream->read((long)count->get_value()));
    }

    NATIVE_FUNCTION(string_stream_readline) {
        EXPECT_NUM_ARGS(0);

        CONVERT_RECV_TO(StringStream, stream);

        return ALLOC_STRING(stream->readline());
    }

    NATIVE_FUNCTION(string_stream_write) {
        EXPECT_NUM_ARGS(1);

        CONVERT_RECV_TO(StringStream, stream);
        CONVERT_ARG_TO(0, String, s);

        stream->write(s->get_value());

        return ALLOC_NUMBER(s->get_value().length());
    }

    MODULE_INITIALIZATION_FUNC(init_io_module) {
        Heap& heap = context->get_heap();
        NativeObjects& native_objects = context->get_native_objects();

        Module* module = ALLOC_MODULE("io");

        FileStream* file_stream = heap.allocate<FileStream>(context, native_objects.get_object_prototype());
        file_stream->set_property(magic_methods::clone, ALLOC_NATIVE_FUNCTION(file_stream_clone));
        file_stream->set_property("open", ALLOC_NATIVE_FUNCTION(file_stream_open));
        file_stream->set_property("is_open", ALLOC_NATIVE_FUNCTION(file_stream_is_open));
        file_stream->set_property("read", ALLOC_NATIVE_FUNCTION(file_stream_read));
        file_stream->set_property("readline", ALLOC_NATIVE_FUNCTION(file_stream_readline));
        file_stream->set_property("write", ALLOC_NATIVE_FUNCTION(file_stream_write));
        module->set_property("FileStream", file_stream);

        Object* file_access = heap.allocate<Object>(context, native_objects.get_object_prototype());
        file_access->set_property("read", ALLOC_STRING("read"));
        file_access->set_property("write", ALLOC_STRING("write"));
        file_access->set_property("read_write", ALLOC_STRING("read_write"));
        module->set_property("FileAccess", file_access);

        StringStream* string_stream = heap.allocate<StringStream>(context, native_objects.get_object_prototype());
        string_stream->set_property(magic_methods::clone, ALLOC_NATIVE_FUNCTION(string_stream_clone));
        string_stream->set_property("read", ALLOC_NATIVE_FUNCTION(string_stream_read));
        string_stream->set_property("readline", ALLOC_NATIVE_FUNCTION(string_stream_readline));
        string_stream->set_property("write", ALLOC_NATIVE_FUNCTION(string_stream_write));
        module->set_property("StringStream", string_stream);

        return module;
    }

} // namespace modules
} // namespace emerald
