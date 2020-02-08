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

#ifndef _EMERALD_PARSER_H
#define _EMERALD_PARSER_H

#include <memory>
#include <string>
#include <vector>

#include "emerald/ast.h"
#include "emerald/scanner.h"
#include "emerald/source.h"
#include "emerald/reporter.h"
#include "emerald/token.h"

namespace emerald {

    class Parser final {
    public:
        static const std::vector<std::shared_ptr<Statement>> parse(
            std::shared_ptr<Source> source, 
            std::shared_ptr<Reporter> reporter);

    private:
        /* Instance Members */
        Scanner _scanner;
        std::shared_ptr<Reporter> _reporter;

        Parser(std::shared_ptr<Source> source, std::shared_ptr<Reporter> reporter);
        
        std::vector<std::shared_ptr<Statement>> parse();
    
        std::shared_ptr<Statement> parse_statement();
        std::shared_ptr<StatementBlock> parse_statement_block(std::vector<Token::Type> end_tokens);
        std::shared_ptr<DoWhileStatement> parse_do_while_statement();
        std::shared_ptr<Statement> parse_for_statement();
        std::shared_ptr<WhileStatement> parse_while_statement();
        std::shared_ptr<IteStatement> parse_ite_statement();
        std::shared_ptr<DeclarationStatement> parse_declaration_statement();
        std::shared_ptr<FunctionStatement> parse_function_statement();
        std::shared_ptr<ObjectStatement> parse_object_statement();
        std::shared_ptr<TryCatchStatement> parse_try_catch_statement();
        std::shared_ptr<ThrowStatement> parse_throw_statement();
        std::shared_ptr<ReturnStatement> parse_return_statement();
        std::shared_ptr<ImportStatement> parse_import_statement();
        std::shared_ptr<ExpressionStatement> parse_expression_statement();

        std::shared_ptr<Expression> parse_expression();
        std::shared_ptr<Expression> parse_expression(std::shared_ptr<Expression> left, int min_precedence);
        std::shared_ptr<Expression> parse_unary();
        std::shared_ptr<Expression> parse_trailer();
        std::shared_ptr<Expression> parse_primary();
        std::shared_ptr<LValueExpression> parse_lvalue_expression();

        std::shared_ptr<FunctionParameter> parse_function_parameter();
        std::shared_ptr<KeyValuePair> parse_key_value_pair();

        std::shared_ptr<Identifier> parse_identifier();

        void expect(Token::Type type);
        bool lookahead(Token::Type type);
        bool match(Token::Type type);

        std::shared_ptr<SourcePosition> start_pos();
        std::shared_ptr<SourcePosition> peek_start_pos();
        std::shared_ptr<SourcePosition> end_pos(std::shared_ptr<SourcePosition> start);

        void report_unexpected_token(std::shared_ptr<Token> token);
    };

} // namespace emerald

#endif // _EMERALD_PARSER_H
