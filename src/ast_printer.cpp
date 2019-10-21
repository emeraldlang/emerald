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

#include <iostream>

#include "emerald/ast_printer.h"

namespace emerald {

    void ASTPrinter::print(const std::vector<std::shared_ptr<Statement>>& statements) {
        ASTPrinter printer;
        for (const std::shared_ptr<Statement>& statement : statements) {
            printer.Visit(statement);
            if (statement != statements.back()) {
                printer._oss << std::endl;
            }
        }
        std::cout << printer._oss.str() << std::endl;
    }

    ASTPrinter::ASTPrinter()
        : _indentation(0) {}

    void ASTPrinter::VisitStatementBlock(const std::shared_ptr<StatementBlock>& statement_block) {
        if (statement_block->get_statements().empty()) {
            _oss << indent() << "(empty_block)";
        } else {
            start_indentation_block("block");

            for (const auto& statement : statement_block->get_statements()) {
                Visit(statement);
                if (statement != statement_block->get_statements().back()) {
                    _oss << std::endl;
                }
            }

            end_indentation_block();
        }
    }

    void ASTPrinter::VisitDoStatement(const std::shared_ptr<DoStatement>& do_statement) {
        start_indentation_block("do");

        Visit(do_statement->get_block());

        end_indentation_block();
    }

    void ASTPrinter::VisitForStatement(const std::shared_ptr<ForStatement>& for_statement) {
        start_indentation_block("for");

        Visit(for_statement->get_init_statement());
        _oss << std::endl;

        Visit(for_statement->get_to_expression());
        _oss << std::endl;

        if (const std::shared_ptr<Expression>& by_expr = for_statement->get_by_expression()) {
            _oss << std::endl;
            Visit(by_expr);
        }

        Visit(for_statement->get_block());

        end_indentation_block();
    }

    void ASTPrinter::VisitWhileStatement(const std::shared_ptr<WhileStatement>& while_statement) {
        start_indentation_block("while");

        Visit(while_statement->get_conditional_expression());
        _oss << std::endl;

        Visit(while_statement->get_block());

        end_indentation_block();
    }

    void ASTPrinter::VisitIteStatement(const std::shared_ptr<IteStatement>& ite_statement) {
        start_indentation_block("if");

        Visit(ite_statement->get_conditional_expression());
        _oss << std::endl;

        Visit(ite_statement->get_then_block());

        if (const std::shared_ptr<Statement>& else_statement = ite_statement->get_else_statement()) {
            _oss << std::endl;
            Visit(else_statement);
        }

        end_indentation_block();
    }

    void ASTPrinter::VisitPrintStatement(const std::shared_ptr<PrintStatement>& print_statement) {
        start_indentation_block("print");

        for (const std::shared_ptr<Expression>& expr : print_statement->get_expressions()) {
            Visit(expr);
            if (expr != print_statement->get_expressions().back()) {
                _oss << std::endl;
            }
        }

        end_indentation_block();
    }

    void ASTPrinter::VisitDeclarationStatement(const std::shared_ptr<DeclarationStatement>& declaration_statement) {
        start_indentation_block("let");

        _oss << indent() << "(" << declaration_statement->get_identifier() << ")";

        if (const std::shared_ptr<Expression>& init = declaration_statement->get_init_expression()) {
            _oss << std::endl;
            Visit(init);
        }

        end_indentation_block();
    }

    void ASTPrinter::VisitFunctionStatement(const std::shared_ptr<FunctionStatement>& function_statement) {
        start_indentation_block("func");

        _oss << indent() << "(" << function_statement->get_identifier() << ")" << std::endl;

        for (const std::shared_ptr<FunctionParameter>& parameter : function_statement->get_parameters()) {
            Visit(parameter);
            _oss << std::endl;
        }

        Visit(function_statement->get_block());
        
        end_indentation_block();
    }

    void ASTPrinter::VisitObjectStatement(const std::shared_ptr<ObjectStatement>& object_statement) {
        start_indentation_block("object");

        _oss << indent() << "(" << object_statement->get_identifier() << ")"  << std::endl;

        if (const std::shared_ptr<LValueExpression>& parent = object_statement->get_parent()) {
            _oss << std::endl;
            Visit(parent);
        }

        Visit(object_statement->get_block());

        end_indentation_block();
    }

    void ASTPrinter::VisitReturnStatement(const std::shared_ptr<ReturnStatement>& return_statement) {
        start_indentation_block("return");

        Visit(return_statement->get_expression());

        end_indentation_block();
    }

    void ASTPrinter::VisitImportStatement(const std::shared_ptr<ImportStatement>& import_statement) {
        _oss << indent() << "(import " << import_statement->get_module_name() << ")";
    }

    void ASTPrinter::VisitExpressionStatement(const std::shared_ptr<ExpressionStatement>& expression_statement) {
        start_indentation_block("expr_stmt");

        Visit(expression_statement->get_expression());

        end_indentation_block();
    }

    void ASTPrinter::VisitAssignmentExpression(const std::shared_ptr<AssignmentExpression>& assignment_expression) {
        start_indentation_block("assignment_expression");

        Visit(assignment_expression->get_lvalue_expression());
        _oss << std::endl;
        Visit(assignment_expression->get_right_expression());

        end_indentation_block();
    }

    void ASTPrinter::VisitBinaryOp(const std::shared_ptr<BinaryOp>& binary_op) {
        start_indentation_block("binary_op");

        Visit(binary_op->get_left_expression());
        _oss << std::endl << indent() << "(" << binary_op->get_operator()->get_lexeme() << ")" << std::endl;
        Visit(binary_op->get_right_expression());

        end_indentation_block();
    }

    void ASTPrinter::VisitUnaryOp(const std::shared_ptr<UnaryOp>& unary_op) {
        start_indentation_block("unary_op");

        _oss << indent() << "(" << unary_op->get_operator()->get_lexeme() << ")" << std::endl;
        Visit(unary_op->get_expression());

        end_indentation_block();
    }

    void ASTPrinter::VisitCallExpression(const std::shared_ptr<CallExpression>& call_expression) {
        start_indentation_block("call");

        Visit(call_expression->get_callee());

        for (const std::shared_ptr<Expression>& arg : call_expression->get_args()) {
            _oss << std::endl;
            Visit(arg);
        }

        end_indentation_block();
    }

    void ASTPrinter::VisitProperty(const std::shared_ptr<Property>& property) {
        start_indentation_block("property");

        Visit(property->get_object());
        _oss << std::endl;
        Visit(property->get_property());

        end_indentation_block();
    }

    void ASTPrinter::VisitIdentifier(const std::shared_ptr<Identifier>& identifier) {
        _oss << indent() << "(identifier " << identifier->get_identifier() << ")";
    }

    void ASTPrinter::VisitNumberLiteral(const std::shared_ptr<NumberLiteral>& number_literal) {
        _oss << indent() << "(number " << number_literal->get_value() << ")";
    }

    void ASTPrinter::VisitNullLiteral(const std::shared_ptr<NullLiteral>&) {
        _oss << indent() << "(null)" << std::endl;
    }

    void ASTPrinter::VisitStringLiteral(const std::shared_ptr<StringLiteral>& string_literal) {
        _oss << indent() << "(string " << string_literal->get_value() << ")";
    }

    void ASTPrinter::VisitBooleanLiteral(const std::shared_ptr<BooleanLiteral>& boolean_literal) {
        _oss << indent() << "(bool " << boolean_literal->get_value() << ")";
    }

    void ASTPrinter::VisitArrayLiteral(const std::shared_ptr<ArrayLiteral>& array_literal) {
        if (array_literal->get_elements().empty()) {
            _oss << indent() << "(empty_array)";
        } else {
            start_indentation_block("array");

            for (const std::shared_ptr<Expression>& elem : array_literal->get_elements()) {
                Visit(elem);
                if (elem != array_literal->get_elements().back()) {
                    _oss << std::endl;
                }
            }

            end_indentation_block();
        }
    }

    void ASTPrinter::VisitObjectLiteral(const std::shared_ptr<ObjectLiteral>& object_literal) {
        if (object_literal->get_key_value_pairs().empty()) {
            _oss << indent() << "(empty_object)" << std::endl;
        } else {
            start_indentation_block("object");

            for (const std::shared_ptr<KeyValuePair>& key_value_pair: object_literal->get_key_value_pairs()) {
                Visit(key_value_pair);
            }

            end_indentation_block();
        }
    }

    void ASTPrinter::VisitCloneExpression(const std::shared_ptr<CloneExpression>& clone_expression) {
        start_indentation_block("clone");
        
        Visit(clone_expression->get_parent());

        for (const std::shared_ptr<Expression>& arg : clone_expression->get_args()) {
            _oss << std::endl;
            Visit(arg);
        }

        end_indentation_block();
    }
    
    void ASTPrinter::VisitSuperExpression(const std::shared_ptr<SuperExpression>& super_expression) {
        start_indentation_block("super");
        Visit(super_expression->get_object());
        end_indentation_block();
    }

    void ASTPrinter::VisitFunctionParameter(const std::shared_ptr<FunctionParameter>& function_parameter) {
        start_indentation_block("function_parameter");

        _oss << indent() << "(" << function_parameter->get_identifier() << ")";
        if (const std::shared_ptr<Expression>& default_expr = function_parameter->get_default_expr()) {
            Visit(default_expr);
            _oss << std::endl;
        }

        end_indentation_block();
    }

    void ASTPrinter::VisitKeyValuePair(const std::shared_ptr<KeyValuePair>& key_value_pair) {
        start_indentation_block("key_value_pair");

        Visit(key_value_pair->get_key());
        _oss << std::endl;
        Visit(key_value_pair->get_value());

        end_indentation_block();
    }

    void ASTPrinter::start_indentation_block(const std::string& name) {
        _oss << indent() << "(" << name << std::endl;
        _indentation++;
    }
    
    void ASTPrinter::end_indentation_block() {
        _indentation--;
        _oss << std::endl << indent() << ")";
    }

    std::string ASTPrinter::indent() {
        return std::string(_indentation, '.');
    }

} // namespace emerald
