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

#ifndef _EMERALD_SCANNER_H
#define _EMERALD_SCANNER_H

#include <memory>
#include <string>
#include <unordered_map>

#include "emerald/reporter.h"
#include "emerald/source.h"
#include "emerald/token.h"

namespace emerald {

    class Scanner {
        public:
            Scanner(std::shared_ptr<Source> source, std::shared_ptr<Reporter> reporter);
            
            std::shared_ptr<Token> current() const;
            std::shared_ptr<Token> peek() const;

            std::shared_ptr<Token> scan();
        
        private:
            static const std::unordered_map<std::string, Token::Type> _keyword_token_map;
            static const char eof_marker = std::char_traits<char>::eof();

            std::shared_ptr<Source> _source;
            std::shared_ptr<Reporter> _reporter;

            std::shared_ptr<Token> _current;
            std::shared_ptr<Token> _next;

            size_t _cp;
            size_t _line;
            size_t _col;
            char _c;

            std::string _temp_buffer;

            std::shared_ptr<Token> scan_string();
            std::shared_ptr<Token> scan_keyword_or_identifier();
            std::shared_ptr<Token> scan_number(bool seen_period);
            void scan_decimal_number();
            bool scan_hex_number();

            void skip_single_line_comment();
            void skip_white_space();

            void advance();

            void add_to_buffer(char c);
            void add_to_buffer_advance();

            std::shared_ptr<Token> emit(Token::Type type);
            std::shared_ptr<Token> emit(Token::Type type, const std::string& lexeme);
            std::shared_ptr<Token> emit_with_temp_buffer(Token::Type type);
            std::shared_ptr<Token> advance_and_emit(Token::Type token);
            std::shared_ptr<Token> advance_and_emit_cond(char next, Token::Type if_, Token::Type else_);

            Token::Type get_keyword_type_or_identifier();

            bool ishexdigit(char c);
            bool isidentifierchar(char c);

            std::shared_ptr<SourcePosition> get_source_position() const;
  };

} // namespace emerald

#endif // _EMERALD_SCANNER_H
