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

#include "emerald/parser.h"
#include "emerald/strutils.h"

namespace emerald {

    const std::vector<std::shared_ptr<Statement>> Parser::parse(
            std::shared_ptr<Source> source, 
            std::shared_ptr<Reporter> reporter) {
        Parser parser(source, reporter);
        std::vector<std::shared_ptr<Statement>> statements = parser.parse();
        if (reporter->has_errors()) {
            return {};
        }

        return statements;
    }

    Parser::Parser(std::shared_ptr<Source> source, std::shared_ptr<Reporter> reporter)
        : _scanner(source, reporter),
        _reporter(reporter) {}

    std::vector<std::shared_ptr<Statement>> Parser::parse() {
        std::vector<std::shared_ptr<Statement>> statements;

        while (!lookahead(Token::EOSF)) {
            statements.push_back(parse_statement());
        }

        return statements;
    }

    std::shared_ptr<Statement> Parser::parse_statement() {
        switch (_scanner.next()->get_type()) {
        case Token::DO:
            return parse_do_while_statement();
        case Token::FOR:
            return parse_for_statement();
        case Token::WHILE:
            return parse_while_statement();
        case Token::BREAK:
            _scanner.scan();
            return std::make_shared<BreakStatement>(_scanner.current()->get_source_position());
        case Token::CONTINUE:
            _scanner.scan();
            return std::make_shared<ContinueStatement>(_scanner.current()->get_source_position());
        case Token::IF:
            return parse_ite_statement();
        case Token::LET:
            return parse_declaration_statement();
        case Token::DEF:
            return parse_function_statement();
        case Token::OBJECT:
            return parse_object_statement();
        case Token::PROP:
            return parse_prop_statement();
        case Token::TRY:
            return parse_try_catch_statement();
        case Token::THROW:
            return parse_throw_statement();
        case Token::RET:
            return parse_return_statement();
        case Token::IMPORT:
            return parse_import_statement();
        default:
            return parse_expression_statement();
        }
    }

    std::shared_ptr<StatementBlock> Parser::parse_statement_block(std::vector<Token::Type> end_tokens) {
        std::shared_ptr<SourcePosition> start = start_pos();

        std::vector<std::shared_ptr<Statement>> statements;
        while (true) {
            for (Token::Type token_type : end_tokens) {
                if (lookahead(token_type)) {
                    return std::make_shared<StatementBlock>(end_pos(start), statements);
                }
            }

            if (lookahead(Token::EOSF)) {
                return std::make_shared<StatementBlock>(end_pos(start), statements);
            }

            statements.push_back(parse_statement());
        }
    }

    std::shared_ptr<DoWhileStatement> Parser::parse_do_while_statement() {
        expect(Token::DO);

        std::shared_ptr<SourcePosition> start = start_pos();

        std::shared_ptr<StatementBlock> body = parse_statement_block({ Token::END });
        expect(Token::END);

        expect(Token::WHILE);
        std::shared_ptr<Expression> conditional = parse_expression();

        return std::make_shared<DoWhileStatement>(end_pos(start), body, conditional);
    }

    std::shared_ptr<Statement> Parser::parse_for_statement() {
        expect(Token::FOR);

        std::shared_ptr<SourcePosition> start = start_pos();

        expect(Token::LET);
        std::shared_ptr<SourcePosition> start_let = start_pos();

        expect(Token::IDENTIFIER);
        std::string identifier = _scanner.current()->get_lexeme();

        if (match(Token::IN)) {
            std::shared_ptr<Expression> iterator = parse_expression();

            expect(Token::DO);
            std::shared_ptr<StatementBlock> block = parse_statement_block({ Token::END });
            expect(Token::END);

            return std::make_shared<ForInStatement>(end_pos(start), identifier, iterator, block);
        } else {
            std::shared_ptr<Expression> init_expression;
            if (match(Token::ASSIGN)) {
                init_expression = parse_expression();
            }

            std::shared_ptr<DeclarationStatement> init = std::make_shared<DeclarationStatement>(
                end_pos(start_let), identifier, init_expression);

            bool increments;
            if (match(Token::TO)) {
                increments = true;
            } else if (match(Token::DOWNTO)) {
                increments = false;
            } else {
                report_unexpected_token(_scanner.scan());
            }
            std::shared_ptr<Expression> to = parse_expression();

            std::shared_ptr<Expression> by;
            if (match(Token::BY)) {
                by = parse_expression();
            }

            expect(Token::DO);
            std::shared_ptr<StatementBlock> block = parse_statement_block({ Token::END });
            expect(Token::END);

            return std::make_shared<ForStatement>(end_pos(start), init, to, increments, by, block);
        }
    }

    std::shared_ptr<WhileStatement> Parser::parse_while_statement() {
        expect(Token::WHILE);

        std::shared_ptr<SourcePosition> start = start_pos();

        std::shared_ptr<Expression> conditional = parse_expression();
        expect(Token::DO);
        std::shared_ptr<StatementBlock> block = parse_statement_block({ Token::END });
        expect(Token::END);

        return std::make_shared<WhileStatement>(end_pos(start), conditional, block);
    }

    std::shared_ptr<IteStatement> Parser::parse_ite_statement() {
        expect(Token::IF);

        std::shared_ptr<SourcePosition> start = start_pos();

        std::shared_ptr<Expression> conditional = parse_expression();
        expect(Token::THEN);
        std::shared_ptr<StatementBlock> then_block = parse_statement_block({ Token::ELSE, Token::END });

        std::shared_ptr<Statement> else_statement;
        if (match(Token::ELSE)) {
            if (lookahead(Token::IF)) {
                else_statement = parse_ite_statement();
            } else {
                else_statement = parse_statement_block({ Token::END });
                expect(Token::END);
            }
        } else {
            expect(Token::END);
        }

        return std::make_shared<IteStatement>(end_pos(start), conditional, then_block, else_statement);
    }

    std::shared_ptr<DeclarationStatement> Parser::parse_declaration_statement() {
        expect(Token::LET);

        std::shared_ptr<SourcePosition> start = start_pos();

        expect(Token::IDENTIFIER);
        std::string identifier = _scanner.current()->get_lexeme();

        std::shared_ptr<Expression> init_expression;
        if (match(Token::ASSIGN)) {
            init_expression = parse_expression();
        }

        std::shared_ptr<SourcePosition> end = end_pos(start);
        return std::make_shared<DeclarationStatement>(end, identifier, init_expression);
    }

    std::shared_ptr<FunctionStatement> Parser::parse_function_statement() {
        expect(Token::DEF);

        std::shared_ptr<SourcePosition> start = start_pos();

        expect(Token::IDENTIFIER);
        std::string identifier = _scanner.current()->get_lexeme();

        std::vector<std::shared_ptr<FunctionParameter>> parameters;
        if (match(Token::COLON)) {
            bool seen_w_default = false;
            do {
                std::shared_ptr<FunctionParameter> parameter = parse_function_parameter();
                if (parameter->has_default()) {
                    seen_w_default = true;
                } else if (seen_w_default) {
                    _reporter->report(
                        ReportCode::non_default_arg_after_default_arg,
                        ReportCode::format_report(ReportCode::non_default_arg_after_default_arg),
                        parameter->get_source_position());
                }

                parameters.push_back(parameter);
            } while (match(Token::COMMA));
        }

        std::shared_ptr<StatementBlock> block = parse_statement_block({ Token::END });

        expect(Token::END);

        return std::make_shared<FunctionStatement>(end_pos(start), identifier, parameters, block);
    }

    std::shared_ptr<ObjectStatement> Parser::parse_object_statement() {
        expect(Token::OBJECT);

        std::shared_ptr<SourcePosition> start = start_pos();

        expect(Token::IDENTIFIER);
        std::string identifier = _scanner.current()->get_lexeme();

        std::shared_ptr<LValueExpression> parent;
        if (match(Token::CLONES)) {
            parent = parse_lvalue_expression();
        }

        std::shared_ptr<StatementBlock> block = parse_statement_block({ Token::END });
        expect(Token::END);

        return std::make_shared<ObjectStatement>(end_pos(start), identifier, parent, block);
    }

    std::shared_ptr<PropStatement> Parser::parse_prop_statement() {
        expect(Token::PROP);

        std::shared_ptr<SourcePosition> start = start_pos();

        expect(Token::IDENTIFIER);
        std::string identifier = _scanner.current()->get_lexeme();

        expect(Token::GET);
        std::shared_ptr<StatementBlock> getter = parse_statement_block({ Token::END });
        expect(Token::END);

        std::shared_ptr<StatementBlock> setter;
        if (match(Token::SET)) {
            setter = parse_statement_block({ Token::END });
            expect(Token::END);
        }

        expect(Token::END);

        return std::make_shared<PropStatement>(end_pos(start), identifier, getter, setter);
    }

    std::shared_ptr<TryCatchStatement> Parser::parse_try_catch_statement() {
        expect(Token::TRY);

        std::shared_ptr<SourcePosition> start = start_pos();

        std::shared_ptr<StatementBlock> try_block = parse_statement_block({ Token::CATCH });
        expect(Token::CATCH);

        expect(Token::IDENTIFIER);
        std::string exception_identifier = _scanner.current()->get_lexeme();

        std::shared_ptr<StatementBlock> catch_block = parse_statement_block({ Token::END });
        expect(Token::END);

        return std::make_shared<TryCatchStatement>(end_pos(start), try_block, exception_identifier, catch_block);
    }

    std::shared_ptr<ThrowStatement> Parser::parse_throw_statement() {
        expect(Token::THROW);

        std::shared_ptr<SourcePosition> start = start_pos();

        std::shared_ptr<Expression> expression = parse_expression();
        return std::make_shared<ThrowStatement>(end_pos(start), expression);
    }

    std::shared_ptr<ReturnStatement> Parser::parse_return_statement() {
        expect(Token::RET);

        std::shared_ptr<SourcePosition> start = start_pos();

        std::shared_ptr<Expression> expression = parse_expression();
        return std::make_shared<ReturnStatement>(end_pos(start), expression);
    }

    std::shared_ptr<ImportStatement> Parser::parse_import_statement() {
        expect(Token::IMPORT);

        std::shared_ptr<SourcePosition> start = start_pos();

        std::vector<std::string> parts;
        do {
            expect(Token::IDENTIFIER);
            parts.push_back(_scanner.current()->get_lexeme()); 
        } while (match(Token::DOT));

        std::string alias;
        if (match(Token::AS)) {
            expect(Token::IDENTIFIER);
            alias = _scanner.current()->get_lexeme();
        }

        return std::make_shared<ImportStatement>(end_pos(start), strutils::join(parts.begin(), parts.end(), "."), alias);
    }

    std::shared_ptr<ExpressionStatement> Parser::parse_expression_statement() {
        std::shared_ptr<SourcePosition> start = peek_start_pos();

        std::shared_ptr<Expression> expression = parse_expression();
        return std::make_shared<ExpressionStatement>(end_pos(start), expression);
    }

    std::shared_ptr<Expression> Parser::parse_expression() {
        return parse_expression(parse_unary(), 0);
    }

    std::shared_ptr<Expression> Parser::parse_expression(std::shared_ptr<Expression> left, int min_precedence) {
        std::shared_ptr<Token> lookahead = _scanner.next();
        while (lookahead->is_binary_op() && lookahead->get_precedence() >= min_precedence) {
            std::shared_ptr<Token> op = _scanner.scan();

            std::shared_ptr<SourcePosition> start = start_pos();

            std::shared_ptr<Expression> right = parse_unary();
            lookahead = _scanner.next();

            while (lookahead->is_binary_op() && ((lookahead->compare_precedence(op) == 1) 
                || (lookahead->is_right_associative() && lookahead->compare_precedence(op) == 0))) {
                right = parse_expression(right, lookahead->get_precedence());
                lookahead = _scanner.next();
            }

            if (op->is_assignment_op()) {
                if (std::shared_ptr<LValueExpression> lvalue_expression = ASTNode::as<LValueExpression>(left)) {
                    left = std::make_shared<AssignmentExpression>(end_pos(start), lvalue_expression, op, right);
                } else {
                    _reporter->report(
                        ReportCode::invalid_lvalue,
                        ReportCode::format_report(ReportCode::invalid_lvalue),
                        left->get_source_position());
                    left = nullptr;
                }
            } else {
                left = std::make_shared<BinaryOp>(end_pos(start), left, op, right);
            }
        }

        return left;
    }

    std::shared_ptr<Expression> Parser::parse_unary() {
        if (_scanner.next()->is_unary_op()) {
            std::shared_ptr<SourcePosition> start = peek_start_pos();

            std::shared_ptr<Token> op = _scanner.scan();
            std::shared_ptr<Expression> expression = parse_unary();

            return std::make_shared<UnaryOp>(end_pos(start), op, expression);
        }

        return parse_trailer();
    }

    std::shared_ptr<Expression> Parser::parse_trailer() {
        std::shared_ptr<Expression> expr = parse_primary();

        while (true) {
            if (match(Token::LPAREN)) {
                std::shared_ptr<SourcePosition> start = start_pos();

                std::vector<std::shared_ptr<Expression>> args;

                if (!lookahead(Token::RPAREN)) {
                    do {
                        args.push_back(parse_expression());
                    } while (match(Token::COMMA));
                }

                expect(Token::RPAREN);

                expr = std::make_shared<CallExpression>(end_pos(start), expr, args);
            } else if (match(Token::LBRACKET)) {
                std::shared_ptr<SourcePosition> start = start_pos(); 

                std::shared_ptr<Expression> property = parse_expression();

                expect(Token::RBRACKET);

                expr = std::make_shared<Property>(end_pos(start), expr, property);
            } else if (match(Token::DOT)) {
                std::shared_ptr<SourcePosition> start = start_pos();

                expect(Token::IDENTIFIER);

                std::shared_ptr<Token> token = _scanner.current();
                std::shared_ptr<Expression> property = std::make_shared<StringLiteral>(
                    token->get_source_position(),
                    token->get_lexeme());

                expr = std::make_shared<Property>(end_pos(start), expr, property);
            } else {
                break;
            }
        }

        return expr;
    }

    std::shared_ptr<Expression> Parser::parse_primary() {
        std::shared_ptr<Token> token = _scanner.scan();
        
        switch (token->get_type()) {
        case Token::STRING_LITERAL:
            return std::make_shared<StringLiteral>(token->get_source_position(),
                token->get_lexeme());
        case Token::DECIMAL_NUMBER_LITERAL:
            return std::make_shared<NumberLiteral>(token->get_source_position(),
                std::stod(token->get_lexeme()));
        case Token::HEX_NUMBER_LITERAL:
            return std::make_shared<NumberLiteral>(token->get_source_position(),
                std::stol(token->get_lexeme(), nullptr, 16));
        case Token::TRUE_LITERAL:
            return std::make_shared<BooleanLiteral>(token->get_source_position(), true);
        case Token::FALSE_LITERAL:
            return std::make_shared<BooleanLiteral>(token->get_source_position(), false);
        case Token::NULL_LITERAL:
            return std::make_shared<NullLiteral>(token->get_source_position());
        case Token::LBRACKET: {
            std::shared_ptr<SourcePosition> start = start_pos();

            std::vector<std::shared_ptr<Expression>> elements;
            if (!lookahead(Token::RBRACKET)) {
                do {
                    elements.push_back(parse_expression());
                } while (match(Token::COMMA) && !lookahead(Token::RBRACKET));
            }

            expect(Token::RBRACKET);

            return std::make_shared<ArrayLiteral>(end_pos(start), elements);
        }
        case Token::LBRACE: {
            std::shared_ptr<SourcePosition> start = start_pos();

            std::vector<std::shared_ptr<KeyValuePair>> key_value_pairs;
            if (!lookahead(Token::RBRACE)) {
                do {
                    key_value_pairs.push_back(parse_key_value_pair());
                } while (match(Token::COMMA) && !lookahead(Token::RBRACE));
            }

            expect(Token::RBRACE);

            return std::make_shared<ObjectLiteral>(end_pos(start), key_value_pairs);
        }
        case Token::IDENTIFIER:
            return std::make_shared<Identifier>(token->get_source_position(), token->get_lexeme());
        case Token::LPAREN: {
            std::shared_ptr<Expression> expression = parse_expression();
            expect(Token::RPAREN);
            return expression;
        }
        case Token::CLONE: {
            std::shared_ptr<SourcePosition> start = start_pos();

            std::shared_ptr<LValueExpression> parent = parse_lvalue_expression();

            std::vector<std::shared_ptr<Expression>> args;
            if (match(Token::LPAREN)) {
                if (!lookahead(Token::RPAREN)) {
                    do
                    {
                        args.push_back(parse_expression());
                    } while (match(Token::COMMA));
                }

                expect(Token::RPAREN);
            }

            return std::make_shared<CloneExpression>(end_pos(start), parent, args);
        }
        case Token::SELF:
            return std::make_shared<SelfExpression>(token->get_source_position());
        default:
            report_unexpected_token(token);
            return nullptr;
        }
    }

    std::shared_ptr<LValueExpression> Parser::parse_lvalue_expression() {
        expect(Token::IDENTIFIER);

        std::shared_ptr<SourcePosition> start = start_pos();
        std::shared_ptr<LValueExpression> lvalue = std::make_shared<Identifier>(
            _scanner.current()->get_source_position(),
            _scanner.current()->get_lexeme());
        
        while (match(Token::DOT)) {
            expect(Token::IDENTIFIER);
            std::shared_ptr<Expression> property = std::make_shared<StringLiteral>(
                _scanner.current()->get_source_position(),
                _scanner.current()->get_lexeme());

            lvalue = std::make_shared<Property>(end_pos(start), lvalue, property);
        }

        return lvalue;
    }

    std::shared_ptr<FunctionParameter> Parser::parse_function_parameter() {
        expect(Token::IDENTIFIER);

        std::shared_ptr<SourcePosition> start = start_pos();

        std::string identifier = _scanner.current()->get_lexeme();

        std::shared_ptr<Expression> default_expr;
        if (match(Token::ASSIGN)) {
            default_expr = parse_expression();
        }

        return std::make_shared<FunctionParameter>(end_pos(start), identifier, default_expr);
    }

    std::shared_ptr<KeyValuePair> Parser::parse_key_value_pair() {
        std::shared_ptr<SourcePosition> start = start_pos();

        std::shared_ptr<Expression> key;
        std::shared_ptr<Token> token = _scanner.scan();

        switch (token->get_type()) {
        case Token::STRING_LITERAL:
        case Token::DECIMAL_NUMBER_LITERAL:
        case Token::HEX_NUMBER_LITERAL:
        case Token::IDENTIFIER:
            key = std::make_shared<StringLiteral>(
                token->get_source_position(),
                token->get_lexeme());
            break;
        case Token::LBRACKET:
            key = parse_expression();
            expect(Token::RBRACKET);
            break;
        default:
            report_unexpected_token(token);
            break;
        }

        expect(Token::COLON);
        std::shared_ptr<Expression> value = parse_expression();

        return std::make_shared<KeyValuePair>(end_pos(start), key, value);
    }

    std::shared_ptr<Identifier> Parser::parse_identifier() {
        expect(Token::IDENTIFIER);

        std::shared_ptr<Token> token = _scanner.current();
        return std::make_shared<Identifier>(token->get_source_position(), token->get_lexeme());
    }

    void Parser::expect(Token::Type type) {
        std::shared_ptr<Token> token = _scanner.scan();
        if (token->get_type() != type) {
            report_unexpected_token(token);
        }
    }

    bool Parser::lookahead(Token::Type type) {
        return _scanner.next()->get_type() == type;
    }

    bool Parser::match(Token::Type type) {
        if (_scanner.next()->get_type() == type) {
            _scanner.scan();
            return true;
        }

        return false;
    }

    std::shared_ptr<SourcePosition> Parser::start_pos() {
        return _scanner.current()->get_source_position();
    }

    std::shared_ptr<SourcePosition> Parser::peek_start_pos() {
        return _scanner.next()->get_source_position();
    }

    std::shared_ptr<SourcePosition> Parser::end_pos(std::shared_ptr<SourcePosition> start) {
        return start->span_to(_scanner.current()->get_source_position());
    }

    void Parser::report_unexpected_token(std::shared_ptr<Token> token) {
        if (token->get_type() == Token::EOSF) {
            _reporter->report(
                ReportCode::unexpected_eosf,
                ReportCode::format_report(ReportCode::unexpected_eosf),
                token->get_source_position());
        } else {
            _reporter->report(
                ReportCode::unexpected_token,
                ReportCode::format_report(ReportCode::unexpected_token, token->get_lexeme()),
                token->get_source_position());
        }
    }

} // namespace emerald
