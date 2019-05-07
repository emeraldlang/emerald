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

    void Compiler::visit(ForStatement* for_statement) {
        for_statement->get_init_statement()->accept(this);

        size_t beginning = _code->create_label();
        size_t end = _code->create_label();

        write_fs_condition(for_statement);
        _code->write_jmp_false(end);

        _code->bind_label(beginning);
        
        for_statement->get_block()->accept(this);

        if (for_statement->get_by_expression()) {
            for_statement->get_by_expression()->accept(this);
        } else {
            _code->write_new_num(1);
        }

        if (for_statement->increments()) {
            _code->write_add();
        } else {
            _code->write_sub();
        }

        write_fs_condition(for_statement);
        _code->write_jmp_true(beginning);

        _code->bind_label(end);
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
        
    }

    void Compiler::visit(ObjectStatement* /*object_statement*/) {

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
        case Token::SHL:
            _code->write_bit_shl();
            break;
        case Token::SHR:
            _code->write_bit_shr();
            break;
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
        case Token::NOT:
            break;
        case Token::BIT_NOT:
            _code->write_bit_not();
            break;
        case Token::SUB:
            _code->write_neg();
            break;
        default:
            break;
        }
    }

    void Compiler::visit(CallExpression* /*call_expression*/) {}

    void Compiler::visit(Property* property) {
        property->get_object()->accept(this);
        property->get_property()->accept(this);
        _code->write_get_prop();
    }

    void Compiler::visit(Identifier* identifier) {
        _code->write_ldloc(identifier->get_resolved_index());
    }

    void Compiler::visit(NumberLiteral* number_literal) {
        _code->write_new_num(number_literal->get_value());
    }

    void Compiler::visit(NullLiteral*) {}

    void Compiler::visit(StringLiteral* string_literal) {
        _code->write_new_str(string_literal->get_value());
    }

    void Compiler::visit(BooleanLiteral* boolean_literal) {
        _code->write_new_boolean(boolean_literal->get_value());
    }

    void Compiler::visit(ArrayLiteral* /*array_literal*/) {}

    void Compiler::visit(ObjectLiteral* /*object_literal*/) {}

    void Compiler::visit(CloneExpression* clone_expression) {
        if (std::shared_ptr<Expression> parent = clone_expression->get_parent()) {
            parent->accept(this);
        }

        _code->write_clone(true);

        for (std::shared_ptr<Expression> arg : clone_expression->get_args()) {
            arg->accept(this);
        }

        _code->write_init(clone_expression->get_args().length());
    }
    
    void Compiler::visit(SuperExpression* /*super_expression*/) {}
    
    void Compiler::visit(FunctionParameter* /*function_parameter*/) {}

    void Compiler::visit(KeyValuePair* /*key_value_pair*/) {}

    void Compiler::write_fs_condition(ForStatement* for_statement) {
        size_t index = for_statement->get_init_statement()->get_resolved_index();

        for_statement->get_to_expression()->accept(this);
        _code->write_ldloc(index);

        if (for_statement->increments()) {
            _code->write_lt();
        } else {
            _code->write_gt();
        }
    }

} // namespace emerald
