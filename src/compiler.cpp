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
            compiler.Visit(statement);
        }

        if (reporter->has_errors()) {
            return nullptr;
        }

        return compiler._code;
    }

    Compiler::Compiler(std::shared_ptr<Reporter> reporter)
        : _reporter(reporter),
        _code(new Code()) {}

    void Compiler::VisitStatementBlock(const std::shared_ptr<StatementBlock>& statement_block) {
        for (std::shared_ptr<Statement> statement : statement_block->get_statements()) {
            Visit(statement);
        }
    }

    void Compiler::VisitDoWhileStatement(const std::shared_ptr<DoWhileStatement>& do_while_statement) {
        ssize_t condition = code()->create_label();
        size_t beginning = code()->create_label();
        size_t end = code()->create_label();
        _loop_stack.emplace(condition, beginning, end);

        code()->bind_label(beginning);
        Visit(do_while_statement->get_block());
        code()->bind_label(condition);
        Visit(do_while_statement->get_conditional_expression());
        code()->write_jmp_true(beginning);
        code()->bind_label(end);

        _loop_stack.pop();
    }

    void Compiler::VisitForStatement(const std::shared_ptr<ForStatement>& for_statement) {
        size_t condition = code()->create_label();
        size_t beginning = code()->create_label();
        size_t end = code()->create_label();

        _loop_stack.emplace(condition, beginning, end);

        Visit(for_statement->get_init_statement());

        write_fs_condition(for_statement);
        code()->write_jmp_false(end);

        code()->bind_label(beginning);
        
        Visit(for_statement->get_block());

        code()->bind_label(condition);
        if (for_statement->get_by_expression()) {
            Visit(for_statement->get_by_expression());
        } else {
            code()->write_new_num((for_statement->increments()) ? 1 : -1);
        }

        write_fs_load(for_statement);

        code()->write_add();
        write_st(for_statement->get_init_statement()->get_identifier());

        write_fs_condition(for_statement);
        code()->write_jmp_true(beginning);

        code()->bind_label(end);

        _loop_stack.pop();
    }

    void Compiler::VisitForInStatement(const std::shared_ptr<ForInStatement>& for_in_statement) {
        size_t condition = code()->create_label();
        size_t beginning = code()->create_label();
        size_t end = code()->create_label();

        _loop_stack.emplace(condition, beginning, end);

        Visit(for_in_statement->get_iterable());
        code()->write_get_iter();

        code()->write_iter_done();
        code()->write_jmp_true(end);

        code()->write_iter_cur();
        write_st(for_in_statement->get_identifier());

        code()->bind_label(beginning);

        Visit(for_in_statement->get_block());

        code()->write_iter_next();
        write_st(for_in_statement->get_identifier());

        code()->write_iter_done();
        code()->write_jmp_false(beginning);

        code()->bind_label(end);

        _loop_stack.pop();
    }

    void Compiler::VisitWhileStatement(const std::shared_ptr<WhileStatement>& while_statement) {
        size_t condition = code()->create_label();
        size_t beginning = code()->create_label();
        size_t end = code()->create_label();

        _loop_stack.emplace(condition, beginning, end);

        Visit(while_statement->get_conditional_expression());
        code()->write_jmp_false(end);

        code()->bind_label(beginning);
        Visit(while_statement->get_block());
        code()->bind_label(condition);
        Visit(while_statement->get_conditional_expression());
        code()->write_jmp_true(beginning);
        code()->bind_label(end);

        _loop_stack.pop();
    }

    void Compiler::VisitBreakStatement(const std::shared_ptr<BreakStatement>& break_statement) {
        if (_loop_stack.empty()) {
            std::string msg = ReportCode::format_report(ReportCode::illegal_break);
            _reporter->report(
                ReportCode::illegal_break,
                msg,
                break_statement->get_source_position());
            return;
        }

        LoopLabels& labels = _loop_stack.top();
        code()->write_jmp(labels.end);
    }

    void Compiler::VisitContinueStatement(const std::shared_ptr<ContinueStatement>& continue_statement) {
        if (_loop_stack.empty()) {
            std::string msg = ReportCode::format_report(ReportCode::illegal_continue);
            _reporter->report(
                ReportCode::illegal_continue,
                msg,
                continue_statement->get_source_position());
            return;
        }

        LoopLabels& labels = _loop_stack.top();
        code()->write_jmp(labels.condition);
    }

    void Compiler::VisitIteStatement(const std::shared_ptr<IteStatement>& ite_statement) {
        size_t next = code()->create_label();
        size_t end = code()->create_label();

        Visit(ite_statement->get_conditional_expression());
        code()->write_jmp_false(next);

        Visit(ite_statement->get_then_block());
        code()->write_jmp(end);

        code()->bind_label(next);

        if (const std::shared_ptr<Statement>& else_statement = ite_statement->get_else_statement()) {
            Visit(else_statement);
        }

        code()->bind_label(end);
    }

    void Compiler::VisitDeclarationStatement(const std::shared_ptr<DeclarationStatement>& declaration_statement) {
        if (const std::shared_ptr<Expression>& init = declaration_statement->get_init_expression()) {
            Visit(init);
        } else {
            code()->write_null();
        }

        write_st(declaration_statement->get_identifier());
    }

    void Compiler::VisitFunctionStatement(const std::shared_ptr<FunctionStatement>& function_statement) {
        const std::shared_ptr<Code>& parent_code = code();
        push_new_func(function_statement->get_identifier());
        write_st(parent_code, function_statement->get_identifier());

        for (const std::shared_ptr<FunctionParameter>& parameter : function_statement->get_parameters()) {
            Visit(parameter);
        }

        Visit(function_statement->get_block());

        code()->write_null();
        code()->write_ret();

        pop_func();
    }

    void Compiler::VisitObjectStatement(const std::shared_ptr<ObjectStatement>& object_statement) {
        push_new_func(object_statement->get_identifier()); 

        Visit(object_statement->get_block());

        const std::vector<std::string>& locals = code()->get_local_names();
        for (const std::string& local : locals) {
            code()->write_ldloc(local);
            code()->write_new_str(local);
        }

        const std::shared_ptr<LValueExpression>& parent = object_statement->get_parent();
        if (parent) {
            Visit(parent);
        }

        code()->write_new_obj(parent != nullptr, locals.size());
        code()->write_ret();

        pop_func();

        code()->write_call(false, 0);

        write_st(object_statement->get_identifier());
    }

    void Compiler::VisitTryCatchStatement(const std::shared_ptr<TryCatchStatement>& try_catch_statement) {
        size_t start_catch = code()->create_label();
        size_t end_catch = code()->create_label();

        code()->write_enter_try(start_catch);
        Visit(try_catch_statement->get_try_block());
        code()->write_exit_try(end_catch);

        code()->bind_label(start_catch);
        write_st(try_catch_statement->get_exception_identifier());
        Visit(try_catch_statement->get_catch_block());
        code()->bind_label(end_catch);
    }

    void Compiler::VisitThrowStatement(const std::shared_ptr<ThrowStatement>& throw_statement) {
        Visit(throw_statement->get_expression());
        code()->write_throw_exc();
    }

    void Compiler::VisitReturnStatement(const std::shared_ptr<ReturnStatement>& return_statement) {
        if (is_top_level()) {
            std::string msg = ReportCode::format_report(ReportCode::illegal_return);
            _reporter->report(
                ReportCode::illegal_return,
                msg,
                return_statement->get_source_position());
        }

        if (const std::shared_ptr<Expression>& expression = return_statement->get_expression()) {
            Visit(expression);
        }

        code()->write_ret();
    }

    void Compiler::VisitImportStatement(const std::shared_ptr<ImportStatement>& import_statement) {
        const std::string& module_name = import_statement->get_module_name();

        code()->write_import(module_name);

        write_st(module_name);
    }

    void Compiler::VisitExpressionStatement(const std::shared_ptr<ExpressionStatement>& expression_statement) {
        Visit(expression_statement->get_expression());
        code()->write_pop(1);
    }

    void Compiler::VisitAssignmentExpression(const std::shared_ptr<AssignmentExpression>& assignment_expression) {
        const std::shared_ptr<LValueExpression>& lvalue = assignment_expression->get_lvalue_expression();
        const std::shared_ptr<Token> op = assignment_expression->get_operator();

        if (std::shared_ptr<Property> property = ASTNode::as<Property>(lvalue)) {
            if (op->is_comp_assignment_op()) {
                Visit(assignment_expression->get_right_expression());
                VisitPropertyLoad(property);
                write_comp_assign(op);
            } else {
                VisitPropertyStore(property, assignment_expression->get_right_expression());
                VisitPropertyLoad(property);
            }
        } else if (std::shared_ptr<Identifier> identifier = ASTNode::as<Identifier>(lvalue)) { 
            if (op->is_comp_assignment_op()) {
                Visit(assignment_expression->get_right_expression());
                VisitIdentifierLoad(identifier);
                write_comp_assign(op);
            } else {
                VisitIdentifierStore(identifier, assignment_expression->get_right_expression());
                VisitIdentifierLoad(identifier);
            }
        }
    }

    void Compiler::VisitBinaryOp(const std::shared_ptr<BinaryOp>& binary_op) {
        switch (binary_op->get_operator()->get_type()) {
        case Token::LOGIC_AND:
            VisitLogicalAndExpression(binary_op);
            break;
        case Token::LOGIC_OR:
            VisitLogicalOrExpression(binary_op);
            break;
        default:
            VisitArithmeticExpression(binary_op);
            break;
        }
    }

    void Compiler::Compiler::VisitLogicalAndExpression(const std::shared_ptr<BinaryOp> binary_op) {
        size_t end = code()->create_label();

        Visit(binary_op->get_left_expression());
        code()->write_jmp_false_or_pop(end);
        Visit(binary_op->get_right_expression());

        code()->bind_label(end);
    }

    void Compiler::VisitLogicalOrExpression(const std::shared_ptr<BinaryOp>& binary_op) {
        size_t end = code()->create_label();

        Visit(binary_op->get_left_expression());
        code()->write_jmp_true_or_pop(end);
        Visit(binary_op->get_right_expression());

        code()->bind_label(end);
    }

    void Compiler::VisitArithmeticExpression(const std::shared_ptr<BinaryOp>& binary_op) {
        Visit(binary_op->get_right_expression());
        Visit(binary_op->get_left_expression());

        switch (binary_op->get_operator()->get_type()) {
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

    void Compiler::VisitUnaryOp(const std::shared_ptr<UnaryOp>& unary_op) {
        Visit(unary_op->get_expression());

        switch (unary_op->get_operator()->get_type()) {
        case Token::NOT:
            code()->write_log_neg();
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

    void Compiler::VisitCallExpression(const std::shared_ptr<CallExpression>& call_expression) {
        const std::shared_ptr<Expression>& callee = call_expression->get_callee();
        size_t num_args = call_expression->get_args().size();

        for (const std::shared_ptr<Expression>& arg : iterutils::reverse(call_expression->get_args())) {
            Visit(arg);
        }

        if (std::shared_ptr<Property> property = ASTNode::as<Property>(callee)) {
            VisitPropertyLoad(property, true);
            code()->write_call(true, num_args);
        } else {
            Visit(callee);
            code()->write_call(false, num_args);
        }
    }

    void Compiler::VisitProperty(const std::shared_ptr<Property>& property) {
        VisitPropertyLoad(property, false);
    }

    void Compiler::VisitPropertyLoad(const std::shared_ptr<Property>& property, bool push_self_back) {
        Visit(property->get_property());
        Visit(property->get_object());

        code()->write_get_prop(push_self_back);
    }

    void Compiler::VisitPropertyStore(const std::shared_ptr<Property>& property, const std::shared_ptr<Expression>& val, bool push_self_back) {
        Visit(val);

        Visit(property->get_property());
        Visit(property->get_object());

        code()->write_set_prop(push_self_back);
    }

    void Compiler::VisitIdentifier(const std::shared_ptr<Identifier>& identifier) {
        VisitIdentifierLoad(identifier);
    }

    void Compiler::VisitIdentifierLoad(const std::shared_ptr<Identifier>& identifier) {
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

    void Compiler::VisitIdentifierStore(const std::shared_ptr<Identifier>& identifier, const std::shared_ptr<Expression>& val) {
        Visit(val);

        const std::string& name = identifier->get_identifier();
        if (code()->is_local_name(name)) {
            code()->write_stloc(name);
        } else if (code()->is_global_name(name)) {
            code()->write_stgbl(name);
        } else {
            std::string msg = ReportCode::format_report(ReportCode::undeclared_variable, name);
            _reporter->report(
                ReportCode::undeclared_variable,
                msg,
                identifier->get_source_position());
        }
    }

    void Compiler::VisitNumberLiteral(const std::shared_ptr<NumberLiteral>& number_literal) {
        code()->write_new_num(number_literal->get_value());
    }

    void Compiler::VisitNullLiteral(const std::shared_ptr<NullLiteral>&) {
        code()->write_null();
    }

    void Compiler::VisitStringLiteral(const std::shared_ptr<StringLiteral>& string_literal) {
        code()->write_new_str(string_literal->get_value());
    }

    void Compiler::VisitBooleanLiteral(const std::shared_ptr<BooleanLiteral>& boolean_literal) {
        code()->write_new_boolean(boolean_literal->get_value());
    }

    void Compiler::VisitArrayLiteral(const std::shared_ptr<ArrayLiteral>& array_literal) {
        const std::vector<std::shared_ptr<Expression>> elements = array_literal->get_elements();
        for (const std::shared_ptr<Expression>& element : iterutils::reverse(elements)) {
            Visit(element);
        }

        code()->write_new_arr(elements.size());
    }

    void Compiler::VisitObjectLiteral(const std::shared_ptr<ObjectLiteral>& object_literal) {
        const std::vector<std::shared_ptr<KeyValuePair>>& key_value_pairs = object_literal->get_key_value_pairs();
        for (const std::shared_ptr<KeyValuePair>& key_value_pair : iterutils::reverse(key_value_pairs)) {
            Visit(key_value_pair);
        }

        code()->write_new_obj(false, key_value_pairs.size());
    }

    void Compiler::VisitCloneExpression(const std::shared_ptr<CloneExpression>& clone_expression) {
        for (const std::shared_ptr<Expression>& arg : iterutils::reverse(clone_expression->get_args())) {
            Visit(arg);
        }

        Visit(clone_expression->get_parent());
        code()->write_new_obj(true, 0);

        code()->write_init(clone_expression->get_args().size());
    }

    void Compiler::VisitSelfExpression(const std::shared_ptr<SelfExpression>&) {
        code()->write_self();
    }

    void Compiler::VisitFunctionParameter(const std::shared_ptr<FunctionParameter>& function_parameter) {
        size_t skip_default_eval = code()->create_label();
        code()->write_jmp_data(skip_default_eval);

        if (std::shared_ptr<Expression> default_expr = function_parameter->get_default_expr()) {
            Visit(default_expr);
        } else {
            code()->write_null();
        }

        code()->bind_label(skip_default_eval);
        
        code()->write_stloc(function_parameter->get_identifier());
    }

    void Compiler::VisitKeyValuePair(const std::shared_ptr<KeyValuePair>& key_value_pair) {
        Visit(key_value_pair->get_value());
        Visit(key_value_pair->get_key());
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

    bool Compiler::is_code_top_level(const std::shared_ptr<Code>& code) {
        return code == _code;
    }

    void Compiler::write_fs_load(const std::shared_ptr<ForStatement>& for_statement) {
        const std::string& name = for_statement->get_init_statement()->get_identifier();
        if (is_top_level()) {
            code()->write_ldgbl(name);
        } else {
            code()->write_ldloc(name);
        }
    }

    void Compiler::write_fs_condition(const std::shared_ptr<ForStatement>& for_statement) {
        Visit(for_statement->get_to_expression());

        write_fs_load(for_statement);

        if (for_statement->increments()) {
            code()->write_lt();
        } else {
            code()->write_gt();
        }
    }

    void Compiler::write_st(const std::shared_ptr<Code>& code, const std::string& identifier) {
        if (is_code_top_level(code)) {
            code->write_stgbl(identifier);
        } else {
            code->write_stloc(identifier);
        }
    }

    void Compiler::write_st(const std::string& identifier) {
        write_st(code(), identifier);
    }

    void Compiler::write_comp_assign(const std::shared_ptr<Token>& op) {
        switch (op->get_type()) {
        case Token::ASSIGN_ADD:
            code()->write_iadd();
            break;
        case Token::ASSIGN_SUB:
            code()->write_isub();
            break;
        case Token::ASSIGN_MUL:
            code()->write_imul();
            break;
        case Token::ASSIGN_DIV:
            code()->write_idiv();
            break;
        case Token::ASSIGN_MOD:
            code()->write_imod();
            break;
        default:
            break;
        }
    }

} // namespace emerald
