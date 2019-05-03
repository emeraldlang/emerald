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

#include <fstream>

#include "emerald/source.h"

namespace emerald {

    Source::Source(const std::string& name, const std::string& source)
        : _name(name), 
        _source(source) {}

    const std::string& Source::get_name() const { 
        return _name; 
    }

    const std::string& Source::get_source() const { 
        return _source; 
    }

    size_t Source::length() const { 
        return _source.length(); 
    }

    char Source::at(size_t i) const { 
        return _source[i]; 
    }

    char Source::operator[](size_t i) const { 
        return _source[i]; 
    }

    std::shared_ptr<Source> Source::from_file(const std::string& path) {
        std::ifstream stream(path);
        return std::make_shared<Source>(
            path,
            std::string(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>()));
    }

    SourcePosition::SourcePosition(std::shared_ptr<Source> source, size_t line, size_t col)
        : _source(source), 
        _start_line(line),
        _start_col(col), 
        _end_line(line), 
        _end_col(col) {}

    SourcePosition::SourcePosition(std::shared_ptr<Source> source, size_t start_line, size_t start_col,
        size_t end_line, size_t end_col)
        : _source(source), 
        _start_line(start_line), 
        _start_col(start_col), 
        _end_line(end_line), 
        _end_col(end_col) {}

    std::shared_ptr<Source> SourcePosition::get_source() const { 
        return _source; 
    }

    size_t SourcePosition::get_start_line() const { 
        return _start_line; 
    }

    size_t SourcePosition::get_start_col() const { 
        return _start_col; 
    }

    size_t SourcePosition::get_end_line() const { 
        return _end_line;
    }

    size_t SourcePosition::get_end_col() const { return _end_col; }

    std::shared_ptr<SourcePosition> SourcePosition::span_to(std::shared_ptr<SourcePosition> to) const {
        return std::make_shared<SourcePosition>(
            _source,
            _start_line,
            _start_col,
            to->_end_line,
            to->_end_line);
    }

} // namespace emerald
