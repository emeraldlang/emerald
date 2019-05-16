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
        for (std::shared_ptr<Statement> statement : statements) {
            statement->accept(&printer);
        }
        std::cout << printer._oss.str() << std::endl;
    }

    ASTPrinter::ASTPrinter()
        : _indentation(0) {}

    void ASTPrinter::visit(StatementBlock* statement_block) {
        if (statement_block->get_statements().empty()) {
            _oss << indent() << "(empty_block)";
        } else {
            start_indentation_block("block");

            for (const auto& statement : statement_block->get_statements()) {
                statement->accept(this);
                if (statement != statement_block->get_statements().back()) {
                    _oss << std::endl;
                }
            }

            end_indentation_block();
        }
    }

    void ASTPrinter::visit(DoStatement* do_statement) {
        start_indentation_block("do");

        do_statement->get_block()->accept(this);

        end_indentation_block();
    }

    void ASTPrinter::visit(ForStatement* for_statement) {
        start_indentation_block("for");

        for_statement->get_init_statement()->accept(this);
        _oss << std::endl;

        for_statement->get_to_expression()->accept(this);
        _oss << std::endl;

        if (std::shared_ptr<Expression> by_expr = for_statement->get_by_expression()) {
            _oss << std::endl;
            by_expr->accept(this);
        }

        for_statement->get_block()->accept(this);

        end_indentation_block();
    }

    void ASTPrinter::visit(WhileStatement* while_statement) {
        start_indentation_block("while");

        while_statement->get_conditional_expression()->accept(this);
        _oss << std::endl;

        while_statement->get_block()->accept(this);

        end_indentation_block();
    }

    void ASTPrinter::visit(IteStatement* ite_statement) {
        start_indentation_block("if");

        ite_statement->get_conditional_expression()->accept(this);
        _oss << std::endl;

        ite_statement->get_then_block()->accept(this);

        if (std::shared_ptr<Statement> else_statement = ite_statement->get_else_statement()) {
            _oss << std::endl;
            else_statement->accept(this);
        }

        end_indentation_block();
    }

    void ASTPrinter::visit(PrintStatement* print_statement) {
        start_indentation_block("print");

        for (std::shared_ptr<Expression> expr : print_statement->get_expressions()) {
            expr->accept(this);
            if (expr != print_statement->get_expressions().back()) {
                _oss << std::endl;
            }
        }

        end_indentation_block();
    }

    void ASTPrinter::visit(DeclarationStatement* declaration_statement) {
        start_indentation_block("let");

        _oss << indent() << "(" << declaration_statement->get_identifier() << ")"  << std::endl;

        if (std::shared_ptr<Expression> init = declaration_statement->get_init_expression()) {
            _oss << std::endl;
            init->accept(this);
        }

        end_indentation_block();
    }

    void ASTPrinter::visit(FunctionStatement* function_statement) {
        start_indentation_block("func");

        _oss << indent() << "(" << function_statement->get_identifier() << ")" << std::endl;

        for (std::shared_ptr<FunctionParameter> parameter : function_statement->get_parameters()) {
            parameter->accept(this);
            _oss << std::endl;
        }

        function_statement->get_block()->accept(this);
        
        end_indentation_block();
    }

    void ASTPrinter::visit(ObjectStatement* object_statement) {
        start_indentation_block("object");

        _oss << indent() << "(" << object_statement->get_identifier() << ")"  << std::endl;

        if (std::shared_ptr<Expression> parent = object_statement->get_parent()) {
            _oss << std::endl;
            parent->accept(this);
        }

        object_statement->get_block()->accept(this);

        end_indentation_block();
    }

    void ASTPrinter::visit(ReturnStatement* return_statement) {
        start_indentation_block("return");
        return_statement->get_expression()->accept(this);
        end_indentation_block();
    }

    void ASTPrinter::visit(ExpressionStatement* expression_statement) {
        start_indentation_block("expr_stmt");
        expression_statement->get_expression()->accept(this);
        end_indentation_block();
    }

    void ASTPrinter::visit(BinaryOp* binary_op) {
        start_indentation_block("binary_op");

        binary_op->get_left_expression()->accept(this);
        _oss << std::endl;

        _oss << indent() << "(" << binary_op->get_operator()->get_lexeme() << ")" << std::endl;

        binary_op->get_right_expression()->accept(this);

        end_indentation_block();
    }

    void ASTPrinter::visit(UnaryOp* unary_op) {
        start_indentation_block("unary_op");

        _oss << indent() << "(" << unary_op->get_operator()->get_lexeme() << ")" << std::endl;

        unary_op->get_expression()->accept(this);

        end_indentation_block();
    }

    void ASTPrinter::visit(CallExpression* call_expression) {
        start_indentation_block("call");

        call_expression->get_callee()->accept(this);

        for (std::shared_ptr<Expression> arg : call_expression->get_args()) {
            _oss << std::endl;
            arg->accept(this);
        }

        end_indentation_block();
    }

    void ASTPrinter::visit(Property* property) {
        start_indentation_block("property");

        property->get_object()->accept(this);
        _oss << std::endl;
        property->get_property()->accept(this);

        end_indentation_block();
    }

    void ASTPrinter::visit(Identifier* identifier) {
        _oss << indent() << "(identifier " << identifier->get_identifier() << ")";
    }

    void ASTPrinter::visit(NumberLiteral* number_literal) {
        _oss << indent() << "(number " << number_literal->get_value() << ")";
    }

    void ASTPrinter::visit(NullLiteral*) {
        _oss << indent() << "(null)" << std::endl;
    }

    void ASTPrinter::visit(StringLiteral* string_literal) {
        _oss << indent() << "(string " << string_literal->get_value() << ")";
    }

    void ASTPrinter::visit(BooleanLiteral* boolean_literal) {
        _oss << indent() << "(bool " << boolean_literal->get_value() << ")";
    }

    void ASTPrinter::visit(ArrayLiteral* array_literal) {
        if (array_literal->get_elements().empty()) {
            _oss << indent() << "(empty_array)";
        } else {
            start_indentation_block("array");

            for (std::shared_ptr<Expression> elem : array_literal->get_elements()) {
                elem->accept(this);
                if (elem != array_literal->get_elements().back()) {
                    _oss << std::endl;
                }
            }

            end_indentation_block();
        }
    }

    void ASTPrinter::visit(ObjectLiteral* object_literal) {
        if (object_literal->get_key_value_pairs().empty()) {
            _oss << indent() << "(empty_object)" << std::endl;
        } else {
            start_indentation_block("object");

            for (std::shared_ptr<KeyValuePair> key_value_pair: object_literal->get_key_value_pairs()) {
                key_value_pair->accept(this);
            }

            end_indentation_block();
        }
    }

    void ASTPrinter::visit(CloneExpression* clone_expression) {
        start_indentation_block("clone");
        
        clone_expression->get_parent()->accept(this);

        for (std::shared_ptr<Expression> arg : clone_expression->get_args()) {
            _oss << std::endl;
            arg->accept(this);
        }

        end_indentation_block();
    }
    
    void ASTPrinter::visit(SuperExpression* super_expression) {
        start_indentation_block("super");
        super_expression->get_object()->accept(this);
        end_indentation_block();
    }

    void ASTPrinter::visit(FunctionParameter* function_parameter) {
        start_indentation_block("function_parameter");

        _oss << indent() << "(" << function_parameter->get_identifier() << ")"  << std::endl;
        if (std::shared_ptr<Expression> default_expr = function_parameter->get_default_expr()) {
            default_expr->accept(this);
        }

        end_indentation_block();
    }

    void ASTPrinter::visit(KeyValuePair* key_value_pair) {
        start_indentation_block("key_value_pair");

        key_value_pair->get_key()->accept(this);
        _oss << std::endl;
        key_value_pair->get_value()->accept(this);

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
