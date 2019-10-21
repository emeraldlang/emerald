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

#ifndef _EMERALD_SOURCE_H
#define _EMERALD_SOURCE_H

#include <filesystem>
#include <memory>
#include <string>

namespace emerald {

    class SourcePosition;

    class Source {
    public:
        Source(const std::string& name, const std::string& source);

        const std::string& get_name() const;

        const std::string& get_source() const;
        size_t length() const;

        char at(size_t i) const;
        char operator[](size_t i) const;

        std::string substr(size_t start, size_t end) const;

        static std::shared_ptr<Source> from_file(const std::filesystem::path& path);

    private:
        std::string _name;
        std::string _source;
    };

    class SourcePosition {
    public:
        SourcePosition(std::shared_ptr<Source> source, size_t line, size_t col);
        SourcePosition(std::shared_ptr<Source> source, size_t start_line, size_t start_col,
            size_t end_line, size_t end_col);

        std::shared_ptr<Source> get_source() const;

        size_t get_start_line() const;
        size_t get_start_col() const;
        size_t get_end_line() const;
        size_t get_end_col() const; 

        std::shared_ptr<SourcePosition> span_to(std::shared_ptr<SourcePosition> to) const;
    private:
        std::shared_ptr<Source> _source;
        size_t _start_line;
        size_t _start_col;
        size_t _end_line;
        size_t _end_col;
    };

} // namespace emerald

#endif // _EMERALD_SOURCE_H
