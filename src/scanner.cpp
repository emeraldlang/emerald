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

#include <locale>

#include "emerald/scanner.h"

namespace emerald {

    Scanner::Scanner(std::shared_ptr<Source> source, std::shared_ptr<Reporter> reporter)
        : _source(source),
        _reporter(reporter),
        _cp(0),
        _line(1),
        _col(1),
        _c(_source->at(_cp)) {
        scan();
    }
            
    std::shared_ptr<Token> Scanner::current() const {
        return _current;
    }

    std::shared_ptr<Token> Scanner::peek() const {
        return _next;
    }

    std::shared_ptr<Token> Scanner::scan() {
        if (_cp >= _source->length()) {
            return emit(Token::EOSF);
        }

        do {
            switch (_c) {
            case ':':
                return advance_and_emit(Token::COLON);
            case '(':
                return advance_and_emit(Token::LPAREN);
            case ')':
                return advance_and_emit(Token::RPAREN);
            case ',':
                return advance_and_emit(Token::COMMA);
            case '[':
                return advance_and_emit(Token::LBRACKET);
            case ']':
                return advance_and_emit(Token::RBRACKET);
            case '.':
                advance();
                if (std::isdigit(_c)) {
                    return scan_number(true);
                }
                return emit(Token::DOT);
            case '{':
                return advance_and_emit(Token::LBRACE);
            case '}':
                return advance_and_emit(Token::RBRACE);
            case '<':
                advance();
                if (_c == '=') {
                    return advance_and_emit(Token::LTE);
                } else if (_c == '<') {
                    return advance_and_emit(Token::SHL);
                }
                return emit(Token::LT);
            case '>':
                advance();
                if (_c == '=') {
                    return advance_and_emit(Token::GTE);
                } else if (_c == '>') {
                    return advance_and_emit(Token::SHR);
                } 
                return emit(Token::GT);
            case '=':
                advance();
                if (_c == '=') {
                    return advance_and_emit(Token::EQ);
                } else if (_c == '>') {
                    return advance_and_emit(Token::ARROW);
                }
                return emit(Token::ASSIGN);
            case '!':
                return advance_and_emit_cond('=', Token::NEQ, Token::NOT);
            case '+':
                return advance_and_emit(Token::ADD);
            case '-':
                return advance_and_emit(Token::SUB);
            case '*':
                return advance_and_emit(Token::MUL);
            case '/':
                return advance_and_emit(Token::DIV);
            case '%':
                return advance_and_emit(Token::MOD);
            case '~':
                return advance_and_emit(Token::BIT_NOT);
            case '|':
                return advance_and_emit_cond('|', Token::LOGIC_OR, Token::BIT_OR);
            case '&':
                return advance_and_emit_cond('&', Token::LOGIC_AND, Token::BIT_AND);
            case '^':
                return advance_and_emit(Token::BIT_XOR);
            case '#':
                skip_single_line_comment();
                break;
            case '"':
            case '\'':
                return scan_string();
            case '_':
                return scan_keyword_or_identifier();
            default:
                if(_cp >= _source->length()) {
                    return emit(Token::EOSF);
                } else if (std::isspace(_c)) {
                    skip_white_space();
                } else if (std::isalpha((*_source)[_cp])) {
                    return scan_keyword_or_identifier();
                } else if (std::isdigit((*_source)[_cp])) {
                    return scan_number(false);
                } else {
                    return advance_and_emit(Token::ILLEGAL);
                }
                break;
            }
        } while (true);
    }

    std::shared_ptr<Token> Scanner::scan_string() {
        char quote = _c;
        advance();

        while (_c != quote) {
            if (_c == eof_marker) {
                return emit(Token::EOSF);
            }
            
            add_to_buffer_advance();
        }

        advance();
        return emit_with_temp_buffer(Token::STRING_LITERAL);
    }

    std::shared_ptr<Token> Scanner::scan_keyword_or_identifier() {
        while (isidentifierchar(_c)) add_to_buffer_advance();

        return emit_with_temp_buffer(get_keyword_type_or_identifier());
    }

    std::shared_ptr<Token> Scanner::scan_number(bool seen_period) {
        if (seen_period) {
            add_to_buffer('.');
            scan_decimal_number();
            return emit_with_temp_buffer(Token::DECIMAL_NUMBER_LITERAL);
        }

        if (_c == '0') {
            if (_c == 'x' || _c == 'X') {
                add_to_buffer_advance();
                if (!scan_hex_number()) return emit(Token::ILLEGAL);
                return emit_with_temp_buffer(Token::HEX_NUMBER_LITERAL);
            }
            // TOOD(zvp): Add octal and binary
        }

        scan_decimal_number();

        if (_c == '.') {
            add_to_buffer_advance();
            scan_decimal_number();
        }
        
        return emit_with_temp_buffer(Token::DECIMAL_NUMBER_LITERAL);
    }

    void Scanner::scan_decimal_number() {
        while (std::isdigit(_c)) {
            add_to_buffer_advance();
        }
    }

    bool Scanner::scan_hex_number() {
        if (!ishexdigit(_c)) return false;
        while (ishexdigit(_c)) {
            add_to_buffer_advance();
        }
        return true;
    }

    void Scanner::skip_single_line_comment() {
        while (_c != '\n')
            advance();
    }

    void Scanner::skip_white_space() {
        while(std::isspace(_c))
            advance();
    }

    void Scanner::advance() {
        if (_cp < _source->length()) {
            if (_source->at(_cp) == '\n') {
                _line++;
                _col = 1;
            } else {
                _col++;
            }

            if (_cp++ == _source->length()) {
                _c = eof_marker;
            } else {
                _c = _source->at(_cp);
            }
        }
    }

    void Scanner::add_to_buffer(char c) {
        _temp_buffer += c;
    }

    void Scanner::add_to_buffer_advance() {
        _temp_buffer += _c;
        advance();
    }
    
    std::shared_ptr<Token> Scanner::emit(Token::Type type) {
        _current = _next;
        _next = std::make_shared<Token>(get_source_position(), type);
        return _current;
    }

    std::shared_ptr<Token> Scanner::emit(Token::Type type, const std::string& lexeme) {
        _current = _next;
        _next = std::make_shared<Token>(get_source_position(), type, lexeme);
        return _current;
    }

    std::shared_ptr<Token> Scanner::emit_with_temp_buffer(Token::Type type) {
        std::shared_ptr<Token> token = emit(type, _temp_buffer);
        _temp_buffer.clear();
        return token; 
    }

    std::shared_ptr<Token> Scanner::advance_and_emit(Token::Type token) {
        advance();
        return emit(token);
    }

    std::shared_ptr<Token> Scanner::advance_and_emit_cond(char next, Token::Type if_, Token::Type else_) {
        advance();
        if (_c == next) {
            advance();
            return emit(if_);
        } else {
            return emit(else_);
        }
    }

    Token::Type Scanner::get_keyword_type_or_identifier() {
        if (_keyword_token_map.find(_temp_buffer) != _keyword_token_map.end()) {
            return _keyword_token_map.at(_temp_buffer);
        }

        return Token::IDENTIFIER;
    }
    
    bool Scanner::ishexdigit(char c) {
        return std::isdigit(c) || ('a' <= c && c <= 'f') || ('A' <= c && c <= 'F');
    }

    bool Scanner::isidentifierchar(char c) {
        return std::isalnum(c) || c == '_';
    }

    std::shared_ptr<SourcePosition> Scanner::get_source_position() const {
        return std::make_shared<SourcePosition>(_source, _line, _col);
    }

    const std::unordered_map<std::string, Token::Type> Scanner::_keyword_token_map = {
        { "let", Token::LET },
        { "if", Token::IF },
        { "else", Token::ELSE },
        { "then", Token::THEN },
        { "for", Token::FOR },
        { "to", Token::TO },
        { "by", Token::BY },
        { "while", Token::WHILE },
        { "def", Token::DEF },
        { "return", Token::RET },
        { "do", Token::DO },
        { "end", Token::END },
        { "print", Token::PRINT },
        { "clone", Token::CLONE },
        { "clones", Token::CLONES },
        { "object", Token::OBJECT },
        { "super", Token::SUPER }
    };

} // namespace emerald
