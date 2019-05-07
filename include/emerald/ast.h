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

#ifndef _EMERALD_AST_H
#define _EMERALD_AST_H

#include <memory>
#include <string>
#include <vector>

#include "emerald/source.h"
#include "emerald/token.h"

namespace emerald {

#define STATEMENT_NODES         \
    X(StatementBlock)           \
    X(DoStatement)              \
    X(ForStatement)             \
    X(WhileStatement)           \
    X(IteStatement)             \
    X(PrintStatement)           \
    X(DeclarationStatement)     \
    X(FunctionStatement)        \
    X(ObjectStatement)          \
    X(ReturnStatement)          \
    X(ExpressionStatement)

#define EXPRESSION_NODES        \
    X(BinaryOp)                 \
    X(UnaryOp)                  \
    X(CallExpression)           \
    X(Property)                 \
    X(Identifier)               \
    X(SelfExpression)           \
    X(NumberLiteral)            \
    X(StringLiteral)            \
    X(BooleanLiteral)           \
    X(NullLiteral)              \
    X(ArrayLiteral)             \
    X(ObjectLiteral)            \
    X(CloneExpression)          \
    X(SuperExpression)

#define AST_NODES               \
    X(FunctionParameter)        \
    X(KeyValuePair)

    /* Forward Declarations */
#define X(NodeType) class NodeType;
    STATEMENT_NODES
    EXPRESSION_NODES
    AST_NODES
#undef X

    class ASTVisitor {
    public:
        virtual void visit(StatementBlock* statement_block) = 0;
        virtual void visit(DoStatement* do_statement) = 0;
        virtual void visit(ForStatement* for_statement) = 0;
        virtual void visit(WhileStatement* while_statement) = 0;
        virtual void visit(IteStatement* ite_statement) = 0;
        virtual void visit(PrintStatement* print_statement) = 0;
        virtual void visit(DeclarationStatement* declaration_statement) = 0;
        virtual void visit(FunctionStatement* function_statement) = 0;
        virtual void visit(ObjectStatement* object_statement) = 0;
        virtual void visit(ReturnStatement* return_statement) = 0;
        virtual void visit(ExpressionStatement* expression_statement) = 0;
        virtual void visit(BinaryOp* binary_op) = 0;
        virtual void visit(UnaryOp* unary_op) = 0;
        virtual void visit(CallExpression* call_expression) = 0;
        virtual void visit(Property* property) = 0;
        virtual void visit(Identifier* identifier) = 0;
        virtual void visit(NumberLiteral* number_literal) = 0;
        virtual void visit(NullLiteral* null_literal) = 0;
        virtual void visit(StringLiteral* string_literal) = 0;
        virtual void visit(BooleanLiteral* boolean_literal) = 0;
        virtual void visit(ArrayLiteral* array_literal) = 0;
        virtual void visit(ObjectLiteral* object_literal) = 0;
        virtual void visit(CloneExpression* clone_expression) = 0;
        virtual void visit(SuperExpression* super_expression) = 0;
        virtual void visit(FunctionParameter* function_parameter) = 0;
        virtual void visit(KeyValuePair* key_value_pair) = 0;
    };

    class Resolvable {
    public:
        Resolvable()
            : _index(0),
            _is_resolved(false) {}
        virtual ~Resolvable() = default;

        size_t get_resolved_index() const {
            return _index;
        }

        void resolve(size_t index) {
            _index = index;
            _is_resolved = true;
        }

        bool is_resolved() const {
            return _is_resolved;
        }

    private:
        size_t _index;
        bool _is_resolved;
    };

    class ResolvableLevel {
    public:
        ResolvableLevel()
            : _index(0),
            _max_num_variables(0),
            _is_resolved(false) {}
        virtual ~ResolvableLevel() = default;

        size_t get_resolved_index() const {
            return _index;
        }

        size_t get_max_num_variables() const {
            return _max_num_variables;
        }

        void resolve(size_t index, size_t max_num_variables) {
            _index = index;
            _max_num_variables = max_num_variables;
            _is_resolved = true;
        }

        bool is_resolved() const {
            return _is_resolved;
        }

    private:
        size_t _index;
        size_t _max_num_variables;
        bool _is_resolved;
    };

    class ASTNode {
    public:
        ASTNode(std::shared_ptr<SourcePosition> position)
            : _position(position) {}
        virtual ~ASTNode() = default;

        std::shared_ptr<SourcePosition> get_source_position() const {
            return _position;
        }

        virtual void accept(ASTVisitor* visitor) = 0;

    private:
        std::shared_ptr<SourcePosition> _position;
    };

    class Statement : public ASTNode {
    public:
        Statement(std::shared_ptr<SourcePosition> position)
            : ASTNode(position) {}
    };

    class Expression : public ASTNode {
    public:
        Expression(std::shared_ptr<SourcePosition> position)
            : ASTNode(position) {}
    };

    class StatementBlock final : public Statement {
    public:
        StatementBlock(std::shared_ptr<SourcePosition> position, std::vector<std::shared_ptr<Statement>> statements) 
            : Statement(position),
            _statements(statements) {}
        
        const std::vector<std::shared_ptr<Statement>>& get_statements() const { return _statements; }

        void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    private:
        std::vector<std::shared_ptr<Statement>> _statements;
    };

    class DoStatement final : public Statement {
    public:
        DoStatement(std::shared_ptr<SourcePosition> position, std::shared_ptr<StatementBlock> block)
            : Statement(position),
            _block(block) {}

        std::shared_ptr<StatementBlock> get_block() const { return _block; }

        void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    private:
        std::shared_ptr<StatementBlock> _block;
    };

    class ForStatement final : public Statement {
    public:
        ForStatement(std::shared_ptr<SourcePosition> position, std::shared_ptr<DeclarationStatement> init, 
            std::shared_ptr<Expression> to, bool increments, std::shared_ptr<Expression> by, std::shared_ptr<StatementBlock> block) 
            : Statement(position),
            _init(init),
            _to(to),
            _increments(increments),
            _by(by),
            _block(block) {}
        
        std::shared_ptr<DeclarationStatement> get_init_statement() const { return _init; }
        std::shared_ptr<Expression> get_to_expression() const { return _to; }
        bool increments() const { return _increments; }
        std::shared_ptr<Expression> get_by_expression() const { return _by; }
        std::shared_ptr<StatementBlock> get_block() const { return _block; }

        void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    private:
        std::shared_ptr<DeclarationStatement> _init;
        std::shared_ptr<Expression> _to;
        bool _increments;
        std::shared_ptr<Expression> _by;
        std::shared_ptr<StatementBlock> _block;
    };

    class WhileStatement final : public Statement {
    public:
        WhileStatement(std::shared_ptr<SourcePosition> position, std::shared_ptr<Expression> conditional, 
            std::shared_ptr<StatementBlock> block) 
            : Statement(position),
            _conditional(conditional),
            _block(block) {}
        
        std::shared_ptr<Expression> get_conditional_expression() const { return _conditional; }
        std::shared_ptr<StatementBlock> get_block() const { return _block; }

        void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    private:
        std::shared_ptr<Expression> _conditional;
        std::shared_ptr<StatementBlock> _block;
    };

    class IteStatement final : public Statement {
    public:
        IteStatement(std::shared_ptr<SourcePosition> position, std::shared_ptr<Expression> conditional, std::shared_ptr<StatementBlock> then_block, 
            std::shared_ptr<Statement> else_statement) 
            : Statement(position),
            _conditional(conditional),
            _then_block(then_block),
            _else_statement(else_statement) {}
        
        std::shared_ptr<Expression> get_conditional_expression() const { return _conditional; }
        std::shared_ptr<StatementBlock> get_then_block() const { return _then_block; }
        std::shared_ptr<Statement> get_else_statement() const { return _else_statement; }

        void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    private:
        std::shared_ptr<Expression> _conditional;
        std::shared_ptr<StatementBlock> _then_block;
        std::shared_ptr<Statement> _else_statement;
    };

    class PrintStatement final : public Statement {
    public:
        PrintStatement(std::shared_ptr<SourcePosition> position, std::vector<std::shared_ptr<Expression>> expressions) 
            : Statement(position),
            _expressions(expressions) {}
        
        const std::vector<std::shared_ptr<Expression>>& get_expressions() const { return _expressions; }

        void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    private:
        std::vector<std::shared_ptr<Expression>> _expressions;
    };

    class DeclarationStatement final : public Statement, public Resolvable {
    public:
        DeclarationStatement(std::shared_ptr<SourcePosition> position, const std::string& identifier, 
            std::shared_ptr<Expression> init_expression)
            : Statement(position),
            _identifier(identifier),
            _init_expression(init_expression) {}
        
        const std::string& get_identifier() const { return _identifier; }
        std::shared_ptr<Expression> get_init_expression() const { return _init_expression; }

        void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    private:
        std::string _identifier;
        std::shared_ptr<Expression> _init_expression;
    };

    class FunctionStatement final : public Statement, public ResolvableLevel {
    public:
        FunctionStatement(std::shared_ptr<SourcePosition> position, const std::string& identifier, 
            std::vector<std::shared_ptr<FunctionParameter>> parameters, std::shared_ptr<StatementBlock> block) 
            : Statement(position),
            _identifier(identifier),
            _parameters(parameters),
            _block(block) {}
        
        const std::string& get_identifier() const { return _identifier; }
        const std::vector<std::shared_ptr<FunctionParameter>>& get_parameters() const { return _parameters; }
        size_t get_arity() const { return _parameters.size(); }
        std::shared_ptr<StatementBlock> get_block() const { return _block; }

        void accept(ASTVisitor* visitor) override { visitor->visit(this); }
        
    private:
        std::string _identifier;
        std::vector<std::shared_ptr<FunctionParameter>> _parameters;
        std::shared_ptr<StatementBlock> _block;
    };

    class ObjectStatement final : public Statement, public Resolvable {
    public:
        ObjectStatement(std::shared_ptr<SourcePosition> position, const std::string& identifier, 
            std::shared_ptr<Expression> parent, const std::vector<std::shared_ptr<Statement>>& properties)
            : Statement(position),
            _identifier(identifier),
            _parent(parent),
            _properties(properties) {}

        const std::string& get_identifier() const { return _identifier; }
        std::shared_ptr<Expression> get_parent() const { return _parent; }
        const std::vector<std::shared_ptr<Statement>>& get_properties() const { return _properties; }
        int get_size() const { return _properties.size(); }

        void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    private:
        std::string _identifier;
        std::shared_ptr<Expression> _parent;
        std::vector<std::shared_ptr<Statement>> _properties;
    };

    class ReturnStatement final : public Statement {
    public:
        ReturnStatement(std::shared_ptr<SourcePosition> position, std::shared_ptr<Expression> expression)
            : Statement(position),
            _expression(expression) {}
        
        std::shared_ptr<Expression> get_expression() const { return _expression; }

        void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    private:
        std::shared_ptr<Expression> _expression;
    };

    class ExpressionStatement final : public Statement {
    public:
        ExpressionStatement(std::shared_ptr<SourcePosition> position, std::shared_ptr<Expression> expression)
            : Statement(position),
            _expression(expression) {}

        std::shared_ptr<Expression> get_expression() const { return _expression; }

        void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    private:
        std::shared_ptr<Expression> _expression;
    };

    class BinaryOp final : public Expression {
    public:
        BinaryOp(std::shared_ptr<SourcePosition> position, std::shared_ptr<Expression> left, std::shared_ptr<Token> op, 
            std::shared_ptr<Expression> right) 
            : Expression(position),
            _left(left),
            _op(op),
            _right(right) {}
         
        std::shared_ptr<Expression> get_left_expression() const { return _left; }
        std::shared_ptr<Token> get_operator() const { return _op; }
        std::shared_ptr<Expression> get_right_expression() const { return _right; }

        void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    private:
        std::shared_ptr<Expression> _left;
        std::shared_ptr<Token> _op;
        std::shared_ptr<Expression> _right;
    };

    class UnaryOp final : public Expression {
    public:
        UnaryOp(std::shared_ptr<SourcePosition> position, std::shared_ptr<Token> op, std::shared_ptr<Expression> expression)
            : Expression(position),
            _op(op),
            _expression(expression) {}
        
        std::shared_ptr<Token> get_operator() const { return _op; }
        std::shared_ptr<Expression> get_expression() const { return _expression; }

        void accept(ASTVisitor* visitor) override { visitor->visit(this); }
    
    private:
        std::shared_ptr<Token> _op;
        std::shared_ptr<Expression> _expression;
    };

    class CallExpression final : public Expression {
    public:
        CallExpression(std::shared_ptr<SourcePosition> position, std::shared_ptr<Expression> callee,
            std::vector<std::shared_ptr<Expression>> args)
            : Expression(position),
            _callee(callee),
            _args(args) {}

        std::shared_ptr<Expression> get_callee() const { return _callee; }
        const std::vector<std::shared_ptr<Expression>>& get_args() const { return _args; }

        void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    private:
        std::shared_ptr<Expression> _callee;
        std::vector<std::shared_ptr<Expression>> _args;
    };

    class Property : public Expression {
    public:
        Property(std::shared_ptr<SourcePosition> position, std::shared_ptr<Expression> object,
            std::shared_ptr<Expression> property)
            : Expression(position),
            _object(object),
            _property(property) {}

        std::shared_ptr<Expression> get_object() const { return _object; }
        std::shared_ptr<Expression> get_property() const { return _property; }

        virtual void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    private:
        std::shared_ptr<Expression> _object;
        std::shared_ptr<Expression> _property;
    };

    class Identifier : public Expression, public Resolvable {
    public:
        Identifier(std::shared_ptr<SourcePosition> position, std::string identifier)
            : Expression(position),
            _identifier(identifier) {}
        
        const std::string& get_identifier() const { return _identifier; }

        virtual void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    private:
        std::string _identifier;
    };

    class NumberLiteral final : public Expression {
    public:
        NumberLiteral(std::shared_ptr<SourcePosition> position, double value) 
            : Expression(position),
            _value(value) {}
        
        double get_value() const { return _value; }

        void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    private:
        double _value;
    };

    class StringLiteral final : public Expression {
    public:
        StringLiteral(std::shared_ptr<SourcePosition> position, std::string value) 
            : Expression(position),
            _value(value) {}
        
        const std::string& get_value() const { return _value; }

        void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    private:
        std::string _value;
    };

    class BooleanLiteral final : public Expression {
    public:
        BooleanLiteral(std::shared_ptr<SourcePosition> position, bool value) 
            : Expression(position),
            _value(value) {}
        
        bool get_value() const { return _value; }

        void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    private:
        bool _value;
    };

    class NullLiteral final : public Expression {
    public:
        NullLiteral(std::shared_ptr<SourcePosition> position)
            : Expression(position) {}

        void accept(ASTVisitor* visitor) override { visitor->visit(this); }
    };

    class ArrayLiteral final : public Expression {
    public:
        ArrayLiteral(std::shared_ptr<SourcePosition> position, std::vector<std::shared_ptr<Expression>> elements)
            : Expression(position),
            _elements(elements) {}

        const std::vector<std::shared_ptr<Expression>>& get_elements() const { return _elements; }
        int get_length() const { return _elements.size(); }

        void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    private:
        std::vector<std::shared_ptr<Expression>> _elements;
    };

    class ObjectLiteral final : public Expression {
    public:
        ObjectLiteral(std::shared_ptr<SourcePosition> position, std::vector<std::shared_ptr<KeyValuePair>> key_value_pairs)
            : Expression(position),
            _key_value_pairs(key_value_pairs) {}

        const std::vector<std::shared_ptr<KeyValuePair>>& get_key_value_pairs() { return _key_value_pairs; }
        int get_size() const { return _key_value_pairs.size(); }

        void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    private:
        std::vector<std::shared_ptr<KeyValuePair>> _key_value_pairs;
    };

    class CloneExpression final : public Expression {
    public:
        CloneExpression(std::shared_ptr<SourcePosition> position, std::shared_ptr<Expression> parent,
            std::vector<std::shared_ptr<Expression>> args)
            : Expression(position),
            _parent(parent),
            _args(args) {}

        std::shared_ptr<Expression> get_parent() const { return _parent; }
        const std::vector<std::shared_ptr<Expression>>& get_args() const { return _args; }

        void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    private:
        std::shared_ptr<Expression> _parent;
        std::vector<std::shared_ptr<Expression>> _args;
    };

    class SuperExpression final : public Expression {
    public:
        SuperExpression(std::shared_ptr<SourcePosition> position, std::shared_ptr<Expression> object)
            : Expression(position),
            _object(object) {}

        std::shared_ptr<Expression> get_object() const { return _object; }

        void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    private:
        std::shared_ptr<Expression> _object;
    };

    class FunctionParameter final : public ASTNode, public Resolvable {
    public:
        FunctionParameter(std::shared_ptr<SourcePosition> position, const std::string& identifier,
            std::shared_ptr<Expression> default_expr)
            : ASTNode(position), 
            _identifier(identifier),
            _default_expr(default_expr) {}

        const std::string& get_identifier() const { return _identifier; }
        std::shared_ptr<Expression> get_default_expr() const { return _default_expr; }

        void accept(ASTVisitor* visitor) override { visitor->visit(this); }

    private:
        std::string _identifier;
        std::shared_ptr<Expression> _default_expr;
    };

    class KeyValuePair final : public ASTNode {
    public:
        KeyValuePair(std::shared_ptr<SourcePosition> position, std::shared_ptr<Expression> key, 
            std::shared_ptr<Expression> value)
            : ASTNode(position),
            _key(key),
            _value(value) {}

        std::shared_ptr<Expression> get_key() const { return _key; }
        std::shared_ptr<Expression> get_value() const { return _value; }

        void accept(ASTVisitor* visitor) override { visitor->visit(this); }

        private:
            std::shared_ptr<Expression> _key;
            std::shared_ptr<Expression> _value;
    };

} // namespace emerald

#endif // _EMERALD_AST_H
