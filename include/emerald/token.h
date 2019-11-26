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

#ifndef _EMERALD_TOKEN_H
#define _EMERALD_TOKEN_H

#include <cstdint>
#include <string>

#include "emerald/source.h"

#define TOKENS                          \
    /* Keywords */                      \
    X(LET, "let", 0)                    \
    X(IF, "if", 0)                      \
    X(ELSE, "else", 0)                  \
    X(THEN, "then", 0)                  \
    X(FOR, "for", 0)                    \
    X(TO, "to", 0)                      \
    X(DOWNTO, "downto", 0)              \
    X(BY, "by", 0)                      \
    X(IN, "in", 0)                      \
    X(WHILE, "while", 0)                \
    X(BREAK, "break", 0)                \
    X(CONTINUE, "continue", 0)          \
    X(DEF, "def", 0)                    \
    X(RET, "return", 0)                 \
    X(TRY, "try", 0)                    \
    X(CATCH, "catch", 0)                \
    X(THROW, "throw", 0)                \
    X(DO, "do", 0)                      \
    X(END, "end", 0)                    \
    X(CLONE, "clone", 0)                \
    X(CLONES, "clones", 0)              \
    X(OBJECT, "object", 0)              \
    X(SELF, "self", 0)                  \
    X(IMPORT, "import", 0)              \
    /* Seperators */                    \
    X(COLON, ":", 0)                    \
    X(LPAREN, "(", 0)                   \
    X(RPAREN, ")", 0)                   \
    X(COMMA, ",", 0)                    \
    X(POUND, "#", 0)                    \
    X(LBRACKET, "[", 0)                 \
    X(RBRACKET, "]", 0)                 \
    X(DOT, ".", 0)                      \
    X(LBRACE, "{", 0)                   \
    X(RBRACE, "}", 0)                   \
    X(ARROW, "=>", 0)                   \
    /* Unary Operators */               \
    X(NOT, "!", 0)                      \
    X(BIT_NOT, "~", 0)                  \
    /* Binary Operators */              \
    X(ASSIGN, "=", 1)                   \
    X(ASSIGN_ADD, "+=", 1)              \
    X(ASSIGN_SUB, "-=", 1)              \
    X(ASSIGN_MUL, "*=", 1)              \
    X(ASSIGN_DIV, "/=", 1)              \
    X(ASSIGN_MOD, "%=", 1)              \
    X(LOGIC_OR, "||", 2)                \
    X(LOGIC_AND, "&&", 3)               \
    X(BIT_OR, "|", 4)                   \
    X(BIT_XOR, "^", 5)                  \
    X(BIT_AND, "&", 6)                  \
    X(EQ, "==", 7)                      \
    X(NEQ, "!=", 7)                     \
    X(LT, "<", 8)                       \
    X(GT, ">", 8)                       \
    X(LTE, "<=", 8)                     \
    X(GTE, ">=", 8)                     \
    X(SHL, "<<", 9)                     \
    X(SHR, ">>", 9)                     \
    X(ADD, "+", 10)                     \
    X(SUB, "-", 10)                     \
    X(MUL, "*", 11)                     \
    X(DIV, "/", 11)                     \
    X(MOD, "%", 11)                     \
    /* Literals */                      \
    X(NULL_LITERAL, "None", 0)          \
    X(TRUE_LITERAL, "True", 0)          \
    X(FALSE_LITERAL, "False", 0)        \
    X(STRING_LITERAL, "", 0)            \
    X(DECIMAL_NUMBER_LITERAL, "", 0)    \
    X(HEX_NUMBER_LITERAL, "", 0)        \
    X(IDENTIFIER, "", 0)                \
    /* Scanner */                       \
    X(EOSF, "", 0)                      \
    X(WHITESPACE, "", 0)                \
    X(NONE, "", 0)                      \
    X(ILLEGAL, "", 0)

namespace emerald {

    class Token {
        public:
#define X(name, lexeme, precedence) name,
            enum Type { TOKENS NUM_TOKENS };
#undef X
            Token(std::shared_ptr<SourcePosition> source_position, Type type);
            Token(std::shared_ptr<SourcePosition> source_position, Type type, 
                const std::string& lexeme);

            std::shared_ptr<SourcePosition> get_source_position() const;
            const std::string& get_lexeme() const;
            Type get_type() const;

            uint8_t get_precedence() const;

            int compare_precedence(const Token& other);
            int compare_precedence(std::shared_ptr<Token> other);

            bool is_assignment_op() const;
            bool is_comp_assignment_op() const;
            bool is_binary_op() const;
            bool is_unary_op() const;
            bool is_right_associative() const;

        private:
            static const std::string _lexemes[NUM_TOKENS];
            static const uint8_t _precedence[NUM_TOKENS];

            int compare(uint8_t a, uint8_t b);

            std::shared_ptr<SourcePosition> _source_position;
            Type _type;
            std::string _lexeme;
    };

} // namespace emerald

#endif // _EMERALD_TOKEN_H
