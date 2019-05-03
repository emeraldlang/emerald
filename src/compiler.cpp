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

#include "emerald/compiler.h"
#include "emerald/token.h"

namespace emerald {

    std::shared_ptr<Code> Compiler::compile(const std::vector<std::shared_ptr<Statement>>& statements) {
        Compiler compiler;
        for (std::shared_ptr<Statement> statement : statements) {
            statement->accept(&compiler);
        }

        return compiler._code;
    }

    Compiler::Compiler()
        : _code(new Code()) {}

    void Compiler::visit(StatementBlock* statement_block) {
        for (std::shared_ptr<Statement> statement : statement_block->get_statements()) {
            statement->accept(this);
        }
    }

    void Compiler::visit(DoStatement* do_statement) {
        do_statement->get_block()->accept(this);
    }

    void Compiler::visit(ForStatement* /*for_statement*/) {
        
    }

    void Compiler::visit(WhileStatement* while_statement) {
        size_t beginning = _code->create_label();
        size_t end = _code->create_label();

        while_statement->get_conditional_expression()->accept(this);
        _code->write_jmp_false(end);

        _code->bind_label(beginning);
        while_statement->get_block()->accept(this);
        while_statement->get_conditional_expression()->accept(this);
        _code->write_jmp_true(beginning);

        _code->bind_label(end);
    }

    void Compiler::visit(IteStatement* ite_statement) {
        size_t next = _code->create_label();
        size_t end = _code->create_label();

        ite_statement->get_conditional_expression()->accept(this);
        _code->write_jmp_false(next);

        ite_statement->get_then_block()->accept(this);
        _code->write_jmp(end);

        _code->bind_label(next);

        if (std::shared_ptr<Statement> else_statement = ite_statement->get_else_statement()) {
            else_statement->accept(this);
        }

        _code->bind_label(end);
    }

    void Compiler::visit(PrintStatement* print_statement) {
        for (std::shared_ptr<Expression> expression : print_statement->get_expressions()) {
            expression->accept(this);
            _code->write_print();
        }
    }

    void Compiler::visit(DeclarationStatement* declaration_statement) {
        if (std::shared_ptr<Expression> init = declaration_statement->get_init_expression()) {
            init->accept(this);

            size_t index = declaration_statement->get_resolved_index();
            _code->write_stloc(index);
        }
    }

    void Compiler::visit(FunctionStatement* /*function_statement*/) {
        // function_statement->get_function()->accept(this);

        // size_t index = function_statement->get_resolved()->get_resolved_index();
        // _code->write_stloc(index);
    }

    void Compiler::visit(ObjectStatement* /*object_statement*/) {
        // object_statement->get_clone()->accept(this);

        // size_t index = object_statement->get_resolved()->get_resolved_index();
        // _code->write_stloc(index);
    }

    void Compiler::visit(ReturnStatement* return_statement) {
        if (std::shared_ptr<Expression> expression = return_statement->get_expression()) {
            expression->accept(this);
        }

        _code->write_ret();
    }

    void Compiler::visit(ExpressionStatement* expression_statement) {
        expression_statement->get_expression()->accept(this);
    }

    void Compiler::visit(BinaryOp* binary_op) {
        binary_op->get_left_expression()->accept(this);
        binary_op->get_right_expression()->accept(this);

        switch (binary_op->get_operator()->get_type()) {
        // case Token::LOGIC_OR:
        //     _block->write(OP_LOGIC_OR);
        //     break;
        // case Token::LOGIC_AND:
        //     _block->write(OP_LOGIC_AND);
        //     break;
        case Token::BIT_OR:
            _code->write_bit_or();
            break;
        case Token::BIT_XOR:
            _code->write_bit_xor();
            break;
        case Token::BIT_AND:
            _code->write_bit_and();
            break;
        case Token::EQ:
            _code->write_eq();
            break;
        case Token::NEQ:
            _code->write_neq();
            break;
        case Token::LT:
            _code->write_lt();
            break;
        case Token::GT:
            _code->write_gt();
            break;
        case Token::LTE:
            _code->write_lte();
            break;
        case Token::GTE:
            _code->write_gte();
            break;
        // case Token::SHL:
        //     _block->write(OP_SHL);
        //     break;
        // case Token::SHR:
        //     _block->write(OP_SHR);
        //     break;
        case Token::ADD:
            _code->write_add();
            break;
        case Token::SUB:
            _code->write_sub();
            break;
        case Token::MUL:
            _code->write_mul();
            break;
        case Token::DIV:
            _code->write_div();
            break;
        case Token::MOD:
            _code->write_mod();
            break;
        default:
            break;
        }
    }

    void Compiler::visit(UnaryOp* unary_op) {
        unary_op->get_expression()->accept(this);

        switch (unary_op->get_operator()->get_type()) {
        // case Token::NOT:
        //     _block->write(OP_NOT);
        //     break;
        // case Token::BIT_NOT:
        //     _block->write(OP_BIT_NOT);
        //     break;
        // case Token::SUB:
        //     _block->write(OP_NEGATE);
        //     break;
        // case Token::CLONE:
        //     _block->write(OP_OBJECT, 0, 1);
        //     break;
        default:
            break;
        }
    }

    void Compiler::visit(CallExpression* /*call_expression*/) {}

    void Compiler::visit(Property* property) {
        property->get_object()->accept(this);
        property->get_property()->accept(this);
        _code->write_getprop();
    }

    void Compiler::visit(Identifier* identifier) {
        _code->write_ldloc(identifier->get_resolved_index());
    }

    void Compiler::visit(NumberLiteral* number_literal) {
        _code->write_newnum(number_literal->get_value());
    }

    void Compiler::visit(NullLiteral*) {}

    void Compiler::visit(StringLiteral* string_literal) {
        _code->write_newstr(string_literal->get_value());
    }

    void Compiler::visit(BooleanLiteral*) {}

    void Compiler::visit(ArrayLiteral* /*array_literal*/) {}

    void Compiler::visit(ObjectLiteral* /*object_literal*/) {}

    void Compiler::visit(CloneExpression* /*clone_expression*/) {}
    
    void Compiler::visit(SuperExpression* /*super_expression*/) {}
    
    void Compiler::visit(FunctionParameter* /*function_parameter*/) {}

    void Compiler::visit(KeyValuePair* /*key_value_pair*/) {}

} // namespace emerald
