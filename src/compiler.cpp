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
#include "emerald/iterutils.h"
#include "emerald/token.h"

namespace emerald {

    std::shared_ptr<Code> Compiler::compile(
            const std::vector<std::shared_ptr<Statement>>& statements,
            std::shared_ptr<Reporter> reporter) {
        Compiler compiler(reporter);
        for (std::shared_ptr<Statement> statement : statements) {
            statement->accept(&compiler);
        }

        return compiler._code;
    }

    Compiler::Compiler(std::shared_ptr<Reporter> reporter)
        : _reporter(reporter),
        _code(new Code()) {}

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

        size_t beginning = code()->create_label();
        size_t end = code()->create_label();

        write_fs_condition(for_statement);
        code()->write_jmp_false(end);

        code()->bind_label(beginning);
        
        for_statement->get_block()->accept(this);

        if (for_statement->get_by_expression()) {
            for_statement->get_by_expression()->accept(this);
        } else {
            code()->write_new_num(1);
        }

        write_fs_load_iter(for_statement);

        code()->write_iadd();

        write_fs_condition(for_statement);
        code()->write_jmp_true(beginning);

        code()->bind_label(end);
    }

    void Compiler::visit(WhileStatement* while_statement) {
        size_t beginning = code()->create_label();
        size_t end = code()->create_label();

        while_statement->get_conditional_expression()->accept(this);
        code()->write_jmp_false(end);

        code()->bind_label(beginning);
        while_statement->get_block()->accept(this);
        while_statement->get_conditional_expression()->accept(this);
        code()->write_jmp_true(beginning);

        code()->bind_label(end);
    }

    void Compiler::visit(IteStatement* ite_statement) {
        size_t next = code()->create_label();
        size_t end = code()->create_label();

        ite_statement->get_conditional_expression()->accept(this);
        code()->write_jmp_false(next);

        ite_statement->get_then_block()->accept(this);
        code()->write_jmp(end);

        code()->bind_label(next);

        if (std::shared_ptr<Statement> else_statement = ite_statement->get_else_statement()) {
            else_statement->accept(this);
        }

        code()->bind_label(end);
    }

    void Compiler::visit(PrintStatement* print_statement) {
        for (std::shared_ptr<Expression> expression : print_statement->get_expressions()) {
            expression->accept(this);
            code()->write_print();
        }
    }

    void Compiler::visit(DeclarationStatement* declaration_statement) {
        if (std::shared_ptr<Expression> init = declaration_statement->get_init_expression()) {
            init->accept(this);
        } else {
            // code()->write_null();
        }

        if (is_top_level()) {
            code()->write_stgbl(declaration_statement->get_identifier());
        } else {
            code()->write_stloc(declaration_statement->get_identifier());
        }
    }

    void Compiler::visit(FunctionStatement* function_statement) {
        push_new_func(function_statement->get_identifier());

        for (std::shared_ptr<FunctionParameter> parameter : iterutils::reverse(function_statement->get_parameters())) {
            parameter->accept(this);
        }

        function_statement->get_block()->accept(this);

        pop_func();

        if (is_top_level()) {
            code()->write_stgbl(function_statement->get_identifier());
        } else {
            code()->write_stloc(function_statement->get_identifier());
        }
    }

    void Compiler::visit(ObjectStatement* object_statement) {
        push_new_func(object_statement->get_identifier());

        object_statement->get_block()->accept(this);

        const std::vector<std::string>& locals = code()->get_local_names();
        for (const std::string& local : locals) {
            code()->write_new_str(local);
            code()->write_ldloc(local);
        }

        std::shared_ptr<Expression> parent = object_statement->get_parent();
        if (parent) {
            parent->accept(this);
        }

        code()->write_new_obj(parent != nullptr, locals.size());
        code()->write_ret();

        pop_func();

        code()->write_call(0);
    
        if (is_top_level()) {
            code()->write_stgbl(object_statement->get_identifier());
        } else {
            code()->write_stloc(object_statement->get_identifier());
        }
    }

    void Compiler::visit(ReturnStatement* return_statement) {
        if (is_top_level()) {
            std::string msg = ReportCode::format_report(ReportCode::illegal_return);
            _reporter->report(
                ReportCode::illegal_return,
                msg,
                return_statement->get_source_position());
        }

        if (std::shared_ptr<Expression> expression = return_statement->get_expression()) {
            expression->accept(this);
        }

        code()->write_ret();
    }

    void Compiler::visit(ImportStatement* import_statement) {
        const std::string& module_name = import_statement->get_module_name();

        code()->write_import(module_name);

        if (is_top_level()) {
            code()->write_stgbl(module_name);
        } else {
            code()->write_stloc(module_name);
        }
    }

    void Compiler::visit(ExpressionStatement* expression_statement) {
        expression_statement->get_expression()->accept(this);
    }

    void Compiler::visit(BinaryOp* binary_op) {
        binary_op->get_right_expression()->accept(this);
        binary_op->get_left_expression()->accept(this);

        switch (binary_op->get_operator()->get_type()) {
        // case Token::ASSIGN:
        //     code()->write_copy();
        //     break;
        // case Token::ASSIGN_ADD:
        //     code()->write_iadd();
        //     break;
        // case Token::ASSIGN_SUB:
        //     code()->write_isub();
        //     break;
        // case Token::ASSIGN_MUL:
        //     code()->write_imul();
        //     break;
        // case Token::ASSIGN_DIV:
        //     code()->write_idiv();
        //     break;
        // case Token::ASSIGN_MOD:
        //     code()->write_imod();
        //     break;
        // case Token::LOGIC_OR:
        //     break;
        // case Token::LOGIC_AND:
        //     break;
        case Token::BIT_OR:
            code()->write_bit_or();
            break;
        case Token::BIT_XOR:
            code()->write_bit_xor();
            break;
        case Token::BIT_AND:
            code()->write_bit_and();
            break;
        case Token::EQ:
            code()->write_eq();
            break;
        case Token::NEQ:
            code()->write_neq();
            break;
        case Token::LT:
            code()->write_lt();
            break;
        case Token::GT:
            code()->write_gt();
            break;
        case Token::LTE:
            code()->write_lte();
            break;
        case Token::GTE:
            code()->write_gte();
            break;
        case Token::SHL:
            code()->write_bit_shl();
            break;
        case Token::SHR:
            code()->write_bit_shr();
            break;
        case Token::ADD:
            code()->write_add();
            break;
        case Token::SUB:
            code()->write_sub();
            break;
        case Token::MUL:
            code()->write_mul();
            break;
        case Token::DIV:
            code()->write_div();
            break;
        case Token::MOD:
            code()->write_mod();
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
            code()->write_bit_not();
            break;
        case Token::SUB:
            code()->write_neg();
            break;
        default:
            break;
        }
    }

    void Compiler::visit(CallExpression* call_expression) {
        std::shared_ptr<Expression> callee = call_expression->get_callee();
        size_t num_args;

        for (std::shared_ptr<Expression> arg : iterutils::reverse(call_expression->get_args())) {
            arg->accept(this);
        }

        if (std::shared_ptr<Property> property = std::dynamic_pointer_cast<Property>(callee)) {
            // HACKISH, better way of doing this ?
            property->get_object()->accept(this);
            num_args = call_expression->get_args().size() + 1;
        } else {
            num_args = call_expression->get_args().size();
        }

        callee->accept(this);

        code()->write_call(num_args);
    }

    void Compiler::visit(Property* property) {
        property->get_property()->accept(this);
        property->get_object()->accept(this);

        code()->write_get_prop();
    }

    void Compiler::visit(Identifier* identifier) {
        const std::string& name = identifier->get_identifier();
        if (code()->is_local_name(name)) {
            code()->write_ldloc(name);
        } else if (code()->is_global_name(name)) {
            code()->write_ldgbl(name);
        } else {
            std::string msg = ReportCode::format_report(ReportCode::undeclared_variable, name);
            _reporter->report(
                ReportCode::undeclared_variable,
                msg,
                identifier->get_source_position());
        }
    }

    void Compiler::visit(NumberLiteral* number_literal) {
        code()->write_new_num(number_literal->get_value());
    }

    void Compiler::visit(NullLiteral*) {
        // code()->write_null();
    }

    void Compiler::visit(StringLiteral* string_literal) {
        code()->write_new_str(string_literal->get_value());
    }

    void Compiler::visit(BooleanLiteral* boolean_literal) {
        code()->write_new_boolean(boolean_literal->get_value());
    }

    void Compiler::visit(ArrayLiteral* array_literal) {
        const std::vector<std::shared_ptr<Expression>> elements = array_literal->get_elements();
        for (std::shared_ptr<Expression> element : iterutils::reverse(elements)) {
            element->accept(this);
        }

        code()->write_new_arr(elements.size());
    }

    void Compiler::visit(ObjectLiteral* object_literal) {
        const std::vector<std::shared_ptr<KeyValuePair>>& key_value_pairs = object_literal->get_key_value_pairs();
        for (std::shared_ptr<KeyValuePair> key_value_pair : iterutils::reverse(key_value_pairs)) {
            key_value_pair->accept(this);
        }

        code()->write_new_obj(false, key_value_pairs.size());
    }

    void Compiler::visit(CloneExpression* clone_expression) {
        clone_expression->get_parent()->accept(this);
        code()->write_new_obj(true, 0);

        const std::vector<std::shared_ptr<Expression>> args = clone_expression->get_args();
        for (std::shared_ptr<Expression> arg : args) {
            arg->accept(this);
        }
        code()->write_init(args.size());
    }

    void Compiler::visit(SuperExpression* super_expression) {
        super_expression->get_object()->accept(this);
        code()->write_get_parent();
    }

    void Compiler::visit(ThisExpression*) {
        code()->write_get_this();
    }
    
    void Compiler::visit(FunctionParameter* function_parameter) {
        code()->write_stloc(function_parameter->get_identifier());
    }

    void Compiler::visit(KeyValuePair* key_value_pair) {
        key_value_pair->get_value()->accept(this);
        key_value_pair->get_key()->accept(this);
    }

    void Compiler::push_new_func(const std::string& label) {
        _code_stack.push(code()->write_new_func(label));
    }

    void Compiler::pop_func() {
        _code_stack.pop();
    }

    bool Compiler::is_top_level() {
        return _code_stack.empty();
    }

    void Compiler::write_fs_load_iter(ForStatement* for_statement) {
        const std::string& name = for_statement->get_init_statement()->get_identifier();
        if (is_top_level()) {
            code()->write_ldgbl(name);
        } else {
            code()->write_ldloc(name);
        }
    }

    void Compiler::write_fs_condition(ForStatement* for_statement) {
        for_statement->get_to_expression()->accept(this);

        write_fs_load_iter(for_statement);

        if (for_statement->increments()) {
            code()->write_lt();
        } else {
            code()->write_gt();
        }
    }

} // namespace emerald
