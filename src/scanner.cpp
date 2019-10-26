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

    std::shared_ptr<Token> Scanner::next() const {
        return _next;
    }

    std::shared_ptr<Token> Scanner::scan() {
        if (_cp >= _source->length()) {
            return emit(Token::EOSF);
        }

        do {
            _sp = _cp;
            _start_line = _line;
            _start_col = _col;

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
                return advance_and_emit(Token::DOT);
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
                return advance_and_emit_cond('=', Token::ASSIGN_ADD, Token::ADD);
            case '-':
                advance();
                if (std::isdigit(_c)) {
                    return scan_number();
                } else if (_c == '=') {
                    return advance_and_emit(Token::ASSIGN_SUB);
                }
                return emit(Token::SUB);
            case '*':
                return advance_and_emit_cond('=', Token::ASSIGN_MUL, Token::MUL);
            case '/':
                return advance_and_emit_cond('=', Token::ASSIGN_DIV, Token::DIV);
            case '%':
                return advance_and_emit_cond('=', Token::ASSIGN_MOD, Token::MOD);
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
                    return scan_number();
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
            
            advance();
        }

        advance();
        return emit(Token::STRING_LITERAL, _source->substr(_sp + 1, _cp - 1));
    }

    std::shared_ptr<Token> Scanner::scan_keyword_or_identifier() {
        while (isidentifierchar(_c)) advance();

        Token::Type type;
        std::string lexeme = _source->substr(_sp, _cp);
        if (_keyword_token_map.find(lexeme) != _keyword_token_map.end()) {
            type = _keyword_token_map.at(lexeme);
        } else {
            type = Token::IDENTIFIER;
        }

        return emit(type, lexeme);
    }

    std::shared_ptr<Token> Scanner::scan_number() {
        if (_c == '0') {
            if (_c == 'x' || _c == 'X') {
                advance();
                if (!scan_hex_number()) return emit(Token::ILLEGAL);
                return emit(Token::HEX_NUMBER_LITERAL);
            }
            // TOOD(zvp): Add octal and binary
        }

        scan_decimal_number();

        if (_c == '.') {
            advance();
            scan_decimal_number();
        }

        return emit(Token::DECIMAL_NUMBER_LITERAL);
    }

    void Scanner::scan_decimal_number() {
        while (std::isdigit(_c)) advance();
    }

    bool Scanner::scan_hex_number() {
        if (!ishexdigit(_c)) return false;
        while (ishexdigit(_c)) advance();
        return true;
    }

    void Scanner::skip_single_line_comment() {
        while (_c != '\n') {
            if (_c == eof_marker) {
                return;
            }
            advance();
        }
    }

    void Scanner::skip_white_space() {
        while(std::isspace(_c)) advance();
    }

    void Scanner::advance() {
        if (_cp < _source->length()) {
            if (_source->at(_cp) == '\n') {
                _line++;
                _col = 1;
            } else {
                _col++;
            }

            if (++_cp == _source->length()) {
                _c = eof_marker;
            } else {
                _c = _source->at(_cp);
            }
        }
    }
    
    std::shared_ptr<Token> Scanner::emit(Token::Type type) {
        return emit(type, _source->substr(_sp, _cp));
    }

    std::shared_ptr<Token> Scanner::emit(Token::Type type, const std::string& lexeme) {
        _current = _next;
        _next = std::make_shared<Token>(get_source_position(), type, lexeme);
        return _current;
    }

    std::shared_ptr<Token> Scanner::advance_and_emit(Token::Type token) {
        advance();
        return emit(token);
    }

    std::shared_ptr<Token> Scanner::advance_and_emit_cond(char next, Token::Type if_, Token::Type else_) {
        advance();
        if (_c == next) {
            return advance_and_emit(if_);
        } else {
            return emit(else_);
        }
    }
    
    bool Scanner::ishexdigit(char c) {
        return std::isdigit(c) || ('a' <= c && c <= 'f') || ('A' <= c && c <= 'F');
    }

    bool Scanner::isidentifierchar(char c) {
        return std::isalnum(c) || c == '_';
    }

    std::shared_ptr<SourcePosition> Scanner::get_source_position() const {
        return std::make_shared<SourcePosition>(_source, _start_line, _start_col, _line, _col);
    }

    const std::unordered_map<std::string, Token::Type> Scanner::_keyword_token_map = {
        { "let", Token::LET },
        { "if", Token::IF },
        { "else", Token::ELSE },
        { "then", Token::THEN },
        { "for", Token::FOR },
        { "to", Token::TO },
        { "downto", Token::DOWNTO },
        { "by", Token::BY },
        { "while", Token::WHILE },
        { "break", Token::BREAK },
        { "continue", Token::CONTINUE },
        { "def", Token::DEF },
        { "return", Token::RET },
        { "try", Token::TRY },
        { "catch", Token::CATCH },
        { "throw", Token::THROW },
        { "do", Token::DO },
        { "end", Token::END },
        { "print", Token::PRINT },
        { "clone", Token::CLONE },
        { "clones", Token::CLONES },
        { "object", Token::OBJECT },
        { "import", Token::IMPORT },
        { "None", Token::NULL_LITERAL },
        { "True", Token::TRUE_LITERAL },
        { "False", Token::FALSE_LITERAL }
    };

} // namespace emerald
