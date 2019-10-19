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

#include "emerald/token.h"

namespace emerald {

    Token::Token(std::shared_ptr<SourcePosition> source_position, Type type)
        : _source_position(source_position),
        _type(type),
        _lexeme(_lexemes[_type]) {}

    Token::Token(std::shared_ptr<SourcePosition> source_position, Type type, 
        const std::string& lexeme)
        : _source_position(source_position),
        _type(type),
        _lexeme(lexeme) {}

    std::shared_ptr<SourcePosition> Token::get_source_position() const {
        return _source_position;
    }

    const std::string& Token::get_lexeme() const {
        return _lexeme;
    }

    Token::Type Token::get_type() const {
        return _type;
    }

    uint8_t Token::get_precedence() const {
        return _precedence[_type];
    }

    int Token::compare_precedence(const Token& other) {
        uint8_t a = get_precedence();
        uint8_t b = other.get_precedence();
        return compare(a, b);
    }

    int Token::compare_precedence(std::shared_ptr<Token> other) {
        uint8_t a = get_precedence();
        uint8_t b = other->get_precedence();
        return compare(a, b);
    }

    bool Token::is_assignment_op() const {
        return ASSIGN <= _type && _type <= ASSIGN_MOD;
    }

    bool Token::is_comp_assignment_op() const {
        return ASSIGN_ADD <= _type && _type <= ASSIGN_MOD;
    }

    bool Token::is_binary_op() const {
        return ASSIGN <= _type && _type <= MOD; 
    }

    bool Token::is_unary_op() const {
        return (NOT <= _type && _type <= BIT_NOT) || _type == SUB;
    }

    bool Token::is_right_associative() const {
        return _type == ASSIGN;
    }

    int Token::compare(uint8_t a, uint8_t b) {
        if (a > b) {
            return 1;
        } else if (a < b) {
            return -1;
        } else {
            return 0;
        }
    }

#define X(name, lexeme, precedence) lexeme,
    const std::string Token::_lexemes[NUM_TOKENS] = {TOKENS};
#undef X

#define X(name, lexeme, precedence) precedence,
    const uint8_t Token::_precedence[NUM_TOKENS] = {TOKENS};
#undef X

} // namespace emerald
