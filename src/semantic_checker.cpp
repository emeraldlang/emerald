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

#include <exception>

#include "fmt/format.h"

#include "emerald/semantic_checker.h"

namespace emerald {

    Scope::Scope()
        : _parent(nullptr) {}
    
    Scope::Scope(Scope* parent)
        : _parent(parent) {}

    Scope* Scope::get_parent() const {
        return _parent;
    }

    bool Scope::declare_variable(const std::string& name) {
        if (_variables.find(name) != _variables.end()) {
            return false;
        }

        _variables[name] = get_next_variable_index();
        return true;
    }

    bool Scope::is_declared(const std::string& name) const {
        if (_variables.find(name) != _variables.end()) {
            return true;
        } else if (_parent != nullptr) {
            return _parent->is_declared(name);
        } else {
            return false;
        }
    }

    size_t Scope::get_variable_index(const std::string& name) const {
        if (_variables.find(name) != _variables.end()) {
            return _variables.at(name);
        } else if (_parent != nullptr) {
            return _parent->get_variable_index(name);
        } else {
            throw std::out_of_range(fmt::format("{0} is not declared", name));
        }
    }

    size_t Scope::get_num_variables() const {
        return _variables.size();
    }

    size_t Scope::get_next_variable_index() {
        size_t index = 0;
        Scope* scope = this;
        while (scope != nullptr) {
            index += scope->get_num_variables();
            scope = scope->_parent;
        }

        return index;
    }

    ScopeLevel::ScopeLevel()
        : _parent(nullptr),
        _max_num_variables(0) {}

    ScopeLevel::ScopeLevel(ScopeLevel* parent)
        : _parent(parent),
        _max_num_variables(0) {}

    ScopeLevel* ScopeLevel::get_parent() const {
        return _parent;
    }

    Scope* ScopeLevel::push_scope() {
        Scope* scope;
        if (_scopes.empty()) { 
            scope = new Scope();
        } else {
            scope = new Scope(_scopes.back());
        }

        _scopes.push_back(scope);

        return scope;
    }

    void ScopeLevel::pop_scope() {
        if (_scopes.empty()) return;

        size_t num_variables;
        for (Scope* scope : _scopes) {
            num_variables += scope->get_num_variables();
        }

        if (num_variables > _max_num_variables) {
            _max_num_variables = num_variables;
        }

        delete _scopes.back();
        _scopes.pop_back();
    }

    Scope* ScopeLevel::current_scope() {
        if (_scopes.empty()) return nullptr;
        return _scopes.back();
    }

    size_t ScopeLevel::get_max_num_variables() const {
        return _max_num_variables;
    }

    void SemanticChecker::check(const std::vector<std::shared_ptr<Statement>>& statements,
        std::shared_ptr<Reporter> reporter) {
        SemanticChecker semantic_checker(reporter);
        for (std::shared_ptr<Statement> statement : statements) {
            statement->accept(&semantic_checker);
        }
    }

    SemanticChecker::SemanticChecker(std::shared_ptr<Reporter> reporter)
        : _reporter(reporter),
        _level(new ScopeLevel()) {
        _level->push_scope();
        _context_stack.push(Context::top_level);
    }

    void SemanticChecker::visit(StatementBlock* statement_block) {
        _level->push_scope();

        for (std::shared_ptr<Statement> statement : statement_block->get_statements()) {
            statement->accept(this);
        }

        _level->pop_scope();
    }

    void SemanticChecker::visit(DoStatement* do_statement) {
        do_statement->get_block()->accept(this);
    }

    void SemanticChecker::visit(ForStatement* for_statement) {
        _level->push_scope();

        for_statement->get_init_statement()->accept(this);
        for_statement->get_to_expression()->accept(this);

        if (std::shared_ptr<Expression> by = for_statement->get_by_expression()) {
            by->accept(this);
        }

        for_statement->get_block()->accept(this);

        _level->pop_scope();
    }

    void SemanticChecker::visit(WhileStatement* while_statement) {
        while_statement->get_conditional_expression()->accept(this);
        while_statement->get_block()->accept(this);
    }

    void SemanticChecker::visit(IteStatement* ite_statement) {
        ite_statement->get_conditional_expression()->accept(this);

        ite_statement->get_then_block()->accept(this);

        if (std::shared_ptr<Statement> else_statement = ite_statement->get_else_statement()) {
            else_statement->accept(this);
        }
    }

    void SemanticChecker::visit(PrintStatement* print_statement) {
        for (std::shared_ptr<Expression> expression : print_statement->get_expressions()) {
            expression->accept(this);
        }
    }

    void SemanticChecker::visit(DeclarationStatement* declaration_statement) {
        if (std::shared_ptr<Expression> init = declaration_statement->get_init_expression()) {
            init->accept(this);
        }

        if (current_context() != Context::object) {
            declare_and_resolve(declaration_statement, declaration_statement->get_identifier());
        }
    }

    void SemanticChecker::visit(FunctionStatement* function_statement) {
        if (current_context() != Context::object) {
            declare_and_resolve(function_statement, function_statement->get_identifier());
        }

        enter_new_level();
        enter_context(Context::function);

        _level->push_scope();

        for (std::shared_ptr<FunctionParameter> parameter : function_statement->get_parameters()) {
            parameter->accept(this);
        }

        function_statement->get_block()->accept(this);

        _level->pop_scope();

        exit_context();
        exit_level();
    }

    void SemanticChecker::visit(ObjectStatement* object_statement) {
        declare_and_resolve(object_statement, object_statement->get_identifier());

        if (std::shared_ptr<Expression> parent = object_statement->get_parent()) {
            parent->accept(this);
        }

        enter_context(Context::object);

        for (std::shared_ptr<Statement> property : object_statement->get_properties()) {
            property->accept(this);
        }

        exit_context();
    }

    void SemanticChecker::visit(ReturnStatement* return_statement) {
        if (current_context() != Context::function) {
            _reporter->report(
                ReportCode::illegal_return,
                ReportCode::format_report(ReportCode::illegal_return),
                return_statement->get_source_position());
        }

        return_statement->get_expression()->accept(this);
    }

    void SemanticChecker::visit(ExpressionStatement* expression_statement) {
        expression_statement->get_expression()->accept(this);
    }

    void SemanticChecker::visit(BinaryOp* binary_op) {
        binary_op->get_left_expression()->accept(this);
        binary_op->get_right_expression()->accept(this);
    }

    void SemanticChecker::visit(UnaryOp* unary_op) {
        unary_op->get_expression()->accept(this);
    }

    void SemanticChecker::visit(CallExpression* call_expression) {
        call_expression->get_callee()->accept(this);

        for (std::shared_ptr<Expression> arg : call_expression->get_args()) {
            arg->accept(this);
        }
    }

    void SemanticChecker::visit(Property* property) {
        property->get_object()->accept(this);
        property->get_property()->accept(this);
    }

    void SemanticChecker::visit(Identifier* identifier) {
        const std::string& id = identifier->get_identifier();
        Scope* current_scope = _level->current_scope();
        if (!current_scope->is_declared(id)) {
            _reporter->report(
                ReportCode::undeclared,
                ReportCode::format_report(ReportCode::undeclared, id),
                identifier->get_source_position());
            return;
        }

        identifier->resolve(current_scope->get_variable_index(id));
    }
    
    void SemanticChecker::visit(NumberLiteral*) {}

    void SemanticChecker::visit(NullLiteral*) {}

    void SemanticChecker::visit(StringLiteral*) {}

    void SemanticChecker::visit(BooleanLiteral*) {}

    void SemanticChecker::visit(ArrayLiteral* array_literal) {
        for (std::shared_ptr<Expression> element : array_literal->get_elements()) {
            element->accept(this);
        }
    }

    void SemanticChecker::visit(ObjectLiteral* object_literal) {
        for (std::shared_ptr<KeyValuePair> key_value_pair : object_literal->get_key_value_pairs()) {
            key_value_pair->accept(this);
        }
    }

    void SemanticChecker::visit(CloneExpression* clone_expression) {
        clone_expression->get_parent()->accept(this);
        for (std::shared_ptr<Expression> arg : clone_expression->get_args()) {
            arg->accept(this);
        }
    }
    
    void SemanticChecker::visit(SuperExpression* super_expression) {
        super_expression->get_object()->accept(this);
    }

    void SemanticChecker::visit(FunctionParameter* function_parameter) {
        declare_and_resolve(function_parameter, function_parameter->get_identifier());

        if (std::shared_ptr<Expression> default_expr = function_parameter->get_default_expr()) {
            default_expr->accept(this);
        }
    }
    
    void SemanticChecker::visit(KeyValuePair* key_value_pair) {
        key_value_pair->get_key()->accept(this);
        key_value_pair->get_value()->accept(this);
    }

    void SemanticChecker::declare_and_resolve(Resolvable* resolvable, const std::string& name) {
        Scope* current_scope = _level->current_scope();
        if (!current_scope->declare_variable(name)) {
            _reporter->report(
                ReportCode::duplicate_declaration,
                ReportCode::format_report(ReportCode::duplicate_declaration, name));
            return;
        }

        resolvable->resolve(current_scope->get_variable_index(name));
    }

    void SemanticChecker::enter_new_level() {
        _level = new ScopeLevel(_level);
    }
    
    void SemanticChecker::exit_level() {
        ScopeLevel* parent = _level->get_parent();
        delete _level;
        _level = parent;
    }

    void SemanticChecker::enter_context(Context context) {
        _context_stack.push(context);
    }
    
    void SemanticChecker::exit_context() {
        _context_stack.pop();
    }

    SemanticChecker::Context SemanticChecker::current_context() {
        return _context_stack.top();
    }

} // namespace emerald
