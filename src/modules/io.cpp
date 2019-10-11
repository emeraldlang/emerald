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

#include "emerald/modules/io.h"
#include "emerald/natives/utils.h"
#include "emerald/magic_methods.h"

namespace emerald {
namespace modules {

    FileStream::FileStream()
        : HeapObject() {}

    FileStream::FileStream(Object* parent)
        : HeapObject(parent) {}

    std::string FileStream::as_str() const {
        return "<file_stream>";
    }

    void FileStream::open(const std::string& filename, std::fstream::openmode mode) {
        _stream.open(filename, mode);
    }

    bool FileStream::is_open() const {
        return _stream.is_open();
    }

    std::string FileStream::read(size_t n) {
        char s[n];
        _stream.read(s, n);
        return std::string(s);
    }

    void FileStream::write(const std::string& s) {
        _stream.write(s.data(), s.size());
    }

    StringStream::StringStream()
        : HeapObject() {}

    StringStream::StringStream(Object* parent)
        : HeapObject(parent) {}

    std::string StringStream::as_str() const {
        return "<string_stream>";
    }

    std::string StringStream::read(size_t n) {
        char s[n];
        _stream.read(s, n);
        return std::string(s);
    }

    void StringStream::write(const std::string& s) {
        _stream.write(s.data(), s.size());
    }

#define X(name)                                                             \
    NativeFunction* get_##name() {                                          \
        static NativeFunction func = NativeFunction(std::function(&name));  \
        return &func;                                                       \
    }
    FILE_STREAM_NATIVES
    STRING_STREAM_NATIVES
#undef X

    NATIVE_FUNCTION(file_stream_clone) {
        (void)native_objects;

        EXPECT_NUM_ARGS(1);

        TRY_CONVERT_RECV_TO(FileStream, self);

        return heap->allocate<FileStream>(self);
    }

    NATIVE_FUNCTION(file_stream_open) {
        (void)heap;

        EXPECT_NUM_ARGS(3);

        TRY_CONVERT_RECV_TO(FileStream, stream);
        TRY_CONVERT_ARG_TO(1, String, filename);
        TRY_CONVERT_ARG_TO(2, String, mode);

        std::fstream::openmode openmode;
        for (char c : mode->get_value()) {
            switch (c) {
            case 'r':
                openmode |= std::fstream::in;
                break;
            case 'w':
                openmode |= std::fstream::out | std::fstream::trunc;
                break;
            case 'a':
                openmode |= std::fstream::app;
                break;
            default:
                break;
            }
        }

        stream->open(filename->get_value(), openmode);

        return native_objects->get_boolean(stream->is_open());
    }
    
    NATIVE_FUNCTION(file_stream_is_open) {
        (void)native_objects;

        EXPECT_NUM_ARGS(1);

        TRY_CONVERT_RECV_TO(FileStream, stream);

        return native_objects->get_boolean(stream->is_open());
    }

    NATIVE_FUNCTION(file_stream_read) {
        EXPECT_NUM_ARGS(2);

        TRY_CONVERT_RECV_TO(FileStream, stream);
        TRY_CONVERT_ARG_TO(1, Number, count);

        return heap->allocate<String>(
            native_objects->get_string_prototype(),
            stream->read((long)count->get_value()));
    }

    NATIVE_FUNCTION(file_stream_write) {
        EXPECT_NUM_ARGS(2);

        TRY_CONVERT_RECV_TO(FileStream, stream);
        TRY_CONVERT_ARG_TO(1, String, s);

        stream->write(s->get_value());

        return heap->allocate<Number>(
            native_objects->get_number_prototype(),
            s->get_value().length());
    }

    NATIVE_FUNCTION(string_stream_clone) {
        (void)native_objects;
        
        EXPECT_NUM_ARGS(1);

        TRY_CONVERT_RECV_TO(StringStream, self);

        return heap->allocate<StringStream>(self);
    }

    NATIVE_FUNCTION(string_stream_read) {
        EXPECT_NUM_ARGS(2);

        TRY_CONVERT_RECV_TO(StringStream, stream);
        TRY_CONVERT_ARG_TO(1, Number, count);

        return heap->allocate<String>(
            native_objects->get_string_prototype(),
            stream->read((long)count->get_value()));
    }

    NATIVE_FUNCTION(string_stream_write) {
        EXPECT_NUM_ARGS(2);

        TRY_CONVERT_RECV_TO(StringStream, stream);
        TRY_CONVERT_ARG_TO(1, String, s);

        stream->write(s->get_value());

        return heap->allocate<Number>(
            native_objects->get_number_prototype(),
            s->get_value().length());
    }

    MODULE_INITIALIZATION_FUNC(init_io_module) {
        Module* module = heap->allocate<Module>("io");

        FileStream* file_stream = heap->allocate<FileStream>(
            native_objects->get_object_prototype());
        file_stream->set_property(magic_methods::clone, get_file_stream_clone());
        file_stream->set_property("open", get_file_stream_open());
        file_stream->set_property("is_open", get_file_stream_is_open());
        file_stream->set_property("read", get_file_stream_read());
        file_stream->set_property("write", get_file_stream_write());
        module->set_property("FileStream", file_stream);

        StringStream* string_stream = heap->allocate<StringStream>(
            native_objects->get_object_prototype());
        string_stream->set_property(magic_methods::clone, get_string_stream_clone());
        string_stream->set_property("read", get_string_stream_read());
        string_stream->set_property("write", get_string_stream_write());
        module->set_property("StringStream", string_stream);

        return module;
    }

} // namespace modules
} // namespace emerald
