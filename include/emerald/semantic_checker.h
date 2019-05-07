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

#ifndef _EMERALD_SEMANTIC_CHECKER_H
#define _EMERALD_SEMANTIC_CHECKER_H

#include <memory>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

#include "emerald/ast.h"
#include "emerald/reporter.h"

namespace emerald {

    class Scope final {
    public:
        Scope();
        Scope(Scope* parent);

        Scope* get_parent() const;

        bool declare_variable(const std::string& name);
        bool is_declared(const std::string& name) const;

        size_t get_variable_index(const std::string& name) const;

        size_t get_num_variables() const;

    private:
        Scope* _parent;
        std::unordered_map<std::string, size_t> _variables;

        size_t get_next_variable_index();
    };

    class ScopeLevel final {
    public:
        ScopeLevel();
        ScopeLevel(ScopeLevel* parent);

        ScopeLevel* get_parent() const;

        Scope* push_scope();
        void pop_scope();

        Scope* current_scope();

        size_t get_max_num_variables() const;

    private:
        ScopeLevel* _parent;

        std::vector<Scope*> _scopes;
        size_t _max_num_variables;
    };

    class SemanticChecker final : public ASTVisitor {
    public:
        static void check(const std::vector<std::shared_ptr<Statement>>& statements,
            std::shared_ptr<Reporter> reporter);

    private:
        std::shared_ptr<Reporter> _reporter;
        ScopeLevel* _level;

        enum Context {
            top_level,
            function,
            object
        };
        std::stack<Context> _context_stack;

        SemanticChecker(std::shared_ptr<Reporter> reporter);

        void visit(StatementBlock* statement_block) override;
        void visit(DoStatement* do_statement) override;
        void visit(ForStatement* for_statement) override;
        void visit(WhileStatement* while_statement) override;
        void visit(IteStatement* ite_statement) override;
        void visit(PrintStatement* print_statement) override;
        void visit(DeclarationStatement* declaration_statement) override;
        void visit(FunctionStatement* function_statement) override;
        void visit(ObjectStatement* object_statement) override;
        void visit(ReturnStatement* return_statement) override;
        void visit(ExpressionStatement* expression_statement) override;
        void visit(BinaryOp* binary_op) override;
        void visit(UnaryOp* unary_op) override;
        void visit(CallExpression* call_expression) override;
        void visit(Property* property) override;
        void visit(Identifier* identifier) override;
        void visit(NumberLiteral* number_literal) override;
        void visit(NullLiteral* null_literal) override;
        void visit(StringLiteral* string_literal) override;
        void visit(BooleanLiteral* boolean_literal) override;
        void visit(ArrayLiteral* array_literal) override;
        void visit(ObjectLiteral* object_literal) override;
        void visit(CloneExpression* clone_expression) override;
        void visit(SuperExpression* super_expression) override;
        void visit(FunctionParameter* function_parameter) override;
        void visit(KeyValuePair* key_value_pair) override;

        bool declare(const std::string& name);
        void declare_and_resolve(Resolvable* resolvable, const std::string& name);

        void enter_new_level();
        void exit_level();

        void enter_context(Context context);
        void exit_context();
        Context current_context();
    };

} // namespace emerald

#endif // _EMERALD_SEMANTIC_CHECKER_H
