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
    X(ForInStatement)           \
    X(WhileStatement)           \
    X(BreakStatement)           \
    X(ContinueStatement)        \
    X(IteStatement)             \
    X(DeclarationStatement)     \
    X(FunctionStatement)        \
    X(ObjectStatement)          \
    X(TryCatchStatement)        \
    X(ThrowStatement)           \
    X(ReturnStatement)          \
    X(ImportStatement)          \
    X(ExpressionStatement)

#define EXPRESSION_NODES        \
    X(AssignmentExpression)     \
    X(BinaryOp)                 \
    X(UnaryOp)                  \
    X(CallExpression)           \
    X(Property)                 \
    X(Identifier)               \
    X(NumberLiteral)            \
    X(StringLiteral)            \
    X(BooleanLiteral)           \
    X(NullLiteral)              \
    X(ArrayLiteral)             \
    X(ObjectLiteral)            \
    X(CloneExpression)          \
    X(SelfExpression)

#define AST_NODES           \
    X(FunctionParameter)    \
    X(KeyValuePair)

#define ALL_NODES       \
    STATEMENT_NODES     \
    EXPRESSION_NODES    \
    AST_NODES

    /* Forward Declarations */
#define X(NodeType) class NodeType;
    ALL_NODES
#undef X

    class ASTNode {
    public:
#define X(NodeType) n##NodeType,
        enum Type {
            ALL_NODES
        };
#undef X

        virtual ~ASTNode() = default;

        std::shared_ptr<SourcePosition> get_source_position() const { return _position; }
        Type get_type() const { return _type; }

        template <class T>
        static std::shared_ptr<T> as(const std::shared_ptr<ASTNode>& node) {
            return std::dynamic_pointer_cast<T>(node);
        }

    protected:
        ASTNode(std::shared_ptr<SourcePosition> position, Type type)
            : _position(position),
            _type(type) {}

    private:
        std::shared_ptr<SourcePosition> _position;
        Type _type;
    };

    class Statement : public ASTNode {
    protected:
        Statement(std::shared_ptr<SourcePosition> position, Type type)
            : ASTNode(position, type) {}
    };

    class Expression : public ASTNode {
    protected:
        Expression(std::shared_ptr<SourcePosition> position, Type type)
            : ASTNode(position, type) {}
    };

    class LValueExpression : public Expression {
    protected:
        LValueExpression(std::shared_ptr<SourcePosition> position, Type type)
            : Expression(position, type) {}
    };

    class StatementBlock final : public Statement {
    public:
        StatementBlock(std::shared_ptr<SourcePosition> position, std::vector<std::shared_ptr<Statement>> statements) 
            : Statement(position, nStatementBlock),
            _statements(statements) {}
        
        const std::vector<std::shared_ptr<Statement>>& get_statements() const { return _statements; }

    private:
        std::vector<std::shared_ptr<Statement>> _statements;
    };

    class DoStatement final : public Statement {
    public:
        DoStatement(std::shared_ptr<SourcePosition> position, std::shared_ptr<StatementBlock> block)
            : Statement(position, nDoStatement),
            _block(block) {}

        const std::shared_ptr<StatementBlock>& get_block() const { return _block; }

    private:
        std::shared_ptr<StatementBlock> _block;
    };

    class ForStatement final : public Statement {
    public:
        ForStatement(std::shared_ptr<SourcePosition> position, std::shared_ptr<DeclarationStatement> init, 
            std::shared_ptr<Expression> to, bool increments, std::shared_ptr<Expression> by, std::shared_ptr<StatementBlock> block) 
            : Statement(position, nForStatement),
            _init(init),
            _to(to),
            _increments(increments),
            _by(by),
            _block(block) {}
        
        const std::shared_ptr<DeclarationStatement>& get_init_statement() const { return _init; }
        const std::shared_ptr<Expression>& get_to_expression() const { return _to; }
        bool increments() const { return _increments; }
        const std::shared_ptr<Expression>& get_by_expression() const { return _by; }
        const std::shared_ptr<StatementBlock>& get_block() const { return _block; }

    private:
        std::shared_ptr<DeclarationStatement> _init;
        std::shared_ptr<Expression> _to;
        bool _increments;
        std::shared_ptr<Expression> _by;
        std::shared_ptr<StatementBlock> _block;
    };

    class ForInStatement final : public Statement {
    public:
        ForInStatement(std::shared_ptr<SourcePosition> position, const std::string& identifier,
            std::shared_ptr<Expression> iterable, std::shared_ptr<StatementBlock> block)
            : Statement(position, nForInStatement),
            _identifier(identifier),
            _iterable(iterable),
            _block(block) {}

        const std::string& get_identifier() const { return _identifier; }
        const std::shared_ptr<Expression>& get_iterable() const { return _iterable; }
        const std::shared_ptr<StatementBlock>& get_block() const { return _block; }

    private:
        std::string _identifier;
        std::shared_ptr<Expression> _iterable;
        std::shared_ptr<StatementBlock> _block;
    };

    class WhileStatement final : public Statement {
    public:
        WhileStatement(std::shared_ptr<SourcePosition> position, std::shared_ptr<Expression> conditional, 
            std::shared_ptr<StatementBlock> block) 
            : Statement(position, nWhileStatement),
            _conditional(conditional),
            _block(block) {}
        
        const std::shared_ptr<Expression>& get_conditional_expression() const { return _conditional; }
        const std::shared_ptr<StatementBlock>& get_block() const { return _block; }

    private:
        std::shared_ptr<Expression> _conditional;
        std::shared_ptr<StatementBlock> _block;
    };

    class BreakStatement final : public Statement {
    public:
        BreakStatement(std::shared_ptr<SourcePosition> position)
            : Statement(position, nBreakStatement) {}
    };

    class ContinueStatement final : public Statement {
    public:
        ContinueStatement(std::shared_ptr<SourcePosition> position)
            : Statement(position, nContinueStatement) {}
    };

    class IteStatement final : public Statement {
    public:
        IteStatement(std::shared_ptr<SourcePosition> position, std::shared_ptr<Expression> conditional, std::shared_ptr<StatementBlock> then_block, 
            std::shared_ptr<Statement> else_statement) 
            : Statement(position, nIteStatement),
            _conditional(conditional),
            _then_block(then_block),
            _else_statement(else_statement) {}
        
        const std::shared_ptr<Expression>& get_conditional_expression() const { return _conditional; }
        const std::shared_ptr<StatementBlock>& get_then_block() const { return _then_block; }
        const std::shared_ptr<Statement>& get_else_statement() const { return _else_statement; }

    private:
        std::shared_ptr<Expression> _conditional;
        std::shared_ptr<StatementBlock> _then_block;
        std::shared_ptr<Statement> _else_statement;
    };

    class DeclarationStatement final : public Statement {
    public:
        DeclarationStatement(std::shared_ptr<SourcePosition> position, const std::string& identifier, 
            std::shared_ptr<Expression> init_expression)
            : Statement(position, nDeclarationStatement),
            _identifier(identifier),
            _init_expression(init_expression) {}
        
        const std::string& get_identifier() const { return _identifier; }
        const std::shared_ptr<Expression>& get_init_expression() const { return _init_expression; }

    private:
        std::string _identifier;
        std::shared_ptr<Expression> _init_expression;
    };

    class FunctionStatement final : public Statement {
    public:
        FunctionStatement(std::shared_ptr<SourcePosition> position, const std::string& identifier, 
            std::vector<std::shared_ptr<FunctionParameter>> parameters, std::shared_ptr<StatementBlock> block) 
            : Statement(position, nFunctionStatement),
            _identifier(identifier),
            _parameters(parameters),
            _block(block) {}
        
        const std::string& get_identifier() const { return _identifier; }
        const std::vector<std::shared_ptr<FunctionParameter>>& get_parameters() const { return _parameters; }
        size_t get_arity() const { return _parameters.size(); }
        const std::shared_ptr<StatementBlock>& get_block() const { return _block; }
        
    private:
        std::string _identifier;
        std::vector<std::shared_ptr<FunctionParameter>> _parameters;
        std::shared_ptr<StatementBlock> _block;
    };

    class ObjectStatement final : public Statement {
    public:
        ObjectStatement(std::shared_ptr<SourcePosition> position, const std::string& identifier, 
            std::shared_ptr<LValueExpression> parent, std::shared_ptr<StatementBlock> block)
            : Statement(position, nObjectStatement),
            _identifier(identifier),
            _parent(parent),
            _block(block) {}

        const std::string& get_identifier() const { return _identifier; }
        const std::shared_ptr<LValueExpression>& get_parent() const { return _parent; }
        const std::shared_ptr<StatementBlock>& get_block() const { return _block; }
    private:
        std::string _identifier;
        std::shared_ptr<LValueExpression> _parent;
        std::shared_ptr<StatementBlock> _block;
    };

    class TryCatchStatement final : public Statement {
    public:
        TryCatchStatement(std::shared_ptr<SourcePosition> position, std::shared_ptr<StatementBlock> try_block,
            const std::string& exception_identifier, std::shared_ptr<StatementBlock> catch_block)
            : Statement(position, nTryCatchStatement),
            _try_block(try_block),
            _exception_identifier(exception_identifier),
            _catch_block(catch_block) {}

        const std::shared_ptr<StatementBlock> get_try_block() const { return _try_block; }
        const std::string& get_exception_identifier() const { return _exception_identifier; }
        const std::shared_ptr<StatementBlock> get_catch_block() const { return _catch_block; }

    private:
        std::shared_ptr<StatementBlock> _try_block;
        std::string _exception_identifier;
        std::shared_ptr<StatementBlock> _catch_block;
    };

    class ThrowStatement final : public Statement {
    public:
        ThrowStatement(std::shared_ptr<SourcePosition> position, std::shared_ptr<Expression> expression)
            :  Statement(position, nThrowStatement),
            _expression(expression) {}

        const std::shared_ptr<Expression> get_expression() const { return _expression; }

    private:
        std::shared_ptr<Expression> _expression;
    };

    class ReturnStatement final : public Statement {
    public:
        ReturnStatement(std::shared_ptr<SourcePosition> position, std::shared_ptr<Expression> expression)
            : Statement(position, nReturnStatement),
            _expression(expression) {}
        
        const std::shared_ptr<Expression>& get_expression() const { return _expression; }

    private:
        std::shared_ptr<Expression> _expression;
    };

    class ImportStatement final : public Statement {
    public:
        ImportStatement(std::shared_ptr<SourcePosition> position, const std::string& module_name)
            : Statement(position, nImportStatement),
            _module_name(module_name) {}

        const std::string& get_module_name() const { return _module_name; }

    private:
        std::string _module_name;
    };

    class ExpressionStatement final : public Statement {
    public:
        ExpressionStatement(std::shared_ptr<SourcePosition> position, std::shared_ptr<Expression> expression)
            : Statement(position, nExpressionStatement),
            _expression(expression) {}

        const std::shared_ptr<Expression>& get_expression() const { return _expression; }

    private:
        std::shared_ptr<Expression> _expression;
    };

    class AssignmentExpression : public Expression {
    public:
        AssignmentExpression(std::shared_ptr<SourcePosition> position, std::shared_ptr<LValueExpression> lvalue_expression,
            std::shared_ptr<Token> op, std::shared_ptr<Expression> right)
            : Expression(position, nAssignmentExpression),
            _lvalue_expression(lvalue_expression),
            _op(op),
            _right(right) {}

        const std::shared_ptr<LValueExpression>& get_lvalue_expression() const { return _lvalue_expression; }
        const std::shared_ptr<Token>& get_operator() const { return _op; }
        const std::shared_ptr<Expression>& get_right_expression() const { return _right; }

    private:
        std::shared_ptr<LValueExpression> _lvalue_expression;
        std::shared_ptr<Token> _op;
        std::shared_ptr<Expression> _right;
    };

    class BinaryOp final : public Expression {
    public:
        BinaryOp(std::shared_ptr<SourcePosition> position, std::shared_ptr<Expression> left, std::shared_ptr<Token> op, 
            std::shared_ptr<Expression> right) 
            : Expression(position, nBinaryOp),
            _left(left),
            _op(op),
            _right(right) {}
         
        const std::shared_ptr<Expression>& get_left_expression() const { return _left; }
        const std::shared_ptr<Token>& get_operator() const { return _op; }
        const std::shared_ptr<Expression>& get_right_expression() const { return _right; }

    private:
        std::shared_ptr<Expression> _left;
        std::shared_ptr<Token> _op;
        std::shared_ptr<Expression> _right;
    };

    class UnaryOp final : public Expression {
    public:
        UnaryOp(std::shared_ptr<SourcePosition> position, std::shared_ptr<Token> op, std::shared_ptr<Expression> expression)
            : Expression(position, nUnaryOp),
            _op(op),
            _expression(expression) {}
        
        const std::shared_ptr<Token>& get_operator() const { return _op; }
        const std::shared_ptr<Expression>& get_expression() const { return _expression; }
    
    private:
        std::shared_ptr<Token> _op;
        std::shared_ptr<Expression> _expression;
    };

    class CallExpression final : public Expression {
    public:
        CallExpression(std::shared_ptr<SourcePosition> position, std::shared_ptr<Expression> callee,
            std::vector<std::shared_ptr<Expression>> args)
            : Expression(position, nCallExpression),
            _callee(callee),
            _args(args) {}

        const std::shared_ptr<Expression>& get_callee() const { return _callee; }
        const std::vector<std::shared_ptr<Expression>>& get_args() const { return _args; }

    private:
        std::shared_ptr<Expression> _callee;
        std::vector<std::shared_ptr<Expression>> _args;
    };

    class Property : public LValueExpression {
    public:
        Property(std::shared_ptr<SourcePosition> position, std::shared_ptr<Expression> object,
            std::shared_ptr<Expression> property)
            : LValueExpression(position, nProperty),
            _object(object),
            _property(property) {}

        const std::shared_ptr<Expression>& get_object() const { return _object; }
        const std::shared_ptr<Expression>& get_property() const { return _property; }

    private:
        std::shared_ptr<Expression> _object;
        std::shared_ptr<Expression> _property;
    };

    class Identifier : public LValueExpression {
    public:
        Identifier(std::shared_ptr<SourcePosition> position, const std::string& identifier)
            : LValueExpression(position, nIdentifier),
            _identifier(identifier) {}
        
        const std::string& get_identifier() const { return _identifier; }

    private:
        std::string _identifier;
    };

    class NumberLiteral final : public Expression {
    public:
        NumberLiteral(std::shared_ptr<SourcePosition> position, double value) 
            : Expression(position, nNumberLiteral),
            _value(value) {}
        
        double get_value() const { return _value; }

    private:
        double _value;
    };

    class StringLiteral final : public Expression {
    public:
        StringLiteral(std::shared_ptr<SourcePosition> position, std::string value) 
            : Expression(position, nStringLiteral),
            _value(value) {}
        
        const std::string& get_value() const { return _value; }

    private:
        std::string _value;
    };

    class BooleanLiteral final : public Expression {
    public:
        BooleanLiteral(std::shared_ptr<SourcePosition> position, bool value) 
            : Expression(position, nBooleanLiteral),
            _value(value) {}
        
        bool get_value() const { return _value; }

    private:
        bool _value;
    };

    class NullLiteral final : public Expression {
    public:
        NullLiteral(std::shared_ptr<SourcePosition> position)
            : Expression(position, nNullLiteral) {}
    };

    class ArrayLiteral final : public Expression {
    public:
        ArrayLiteral(std::shared_ptr<SourcePosition> position, std::vector<std::shared_ptr<Expression>> elements)
            : Expression(position, nArrayLiteral),
            _elements(elements) {}

        const std::vector<std::shared_ptr<Expression>>& get_elements() const { return _elements; }
        int get_length() const { return _elements.size(); }

    private:
        std::vector<std::shared_ptr<Expression>> _elements;
    };

    class ObjectLiteral final : public Expression {
    public:
        ObjectLiteral(std::shared_ptr<SourcePosition> position, std::vector<std::shared_ptr<KeyValuePair>> key_value_pairs)
            : Expression(position, nObjectLiteral),
            _key_value_pairs(key_value_pairs) {}

        const std::vector<std::shared_ptr<KeyValuePair>>& get_key_value_pairs() { return _key_value_pairs; }
        int get_size() const { return _key_value_pairs.size(); }

    private:
        std::vector<std::shared_ptr<KeyValuePair>> _key_value_pairs;
    };

    class CloneExpression final : public Expression {
    public:
        CloneExpression(std::shared_ptr<SourcePosition> position, std::shared_ptr<LValueExpression> parent,
            std::vector<std::shared_ptr<Expression>> args)
            : Expression(position, nCloneExpression),
            _parent(parent),
            _args(args) {}

        const std::shared_ptr<LValueExpression>& get_parent() const { return _parent; }
        const std::vector<std::shared_ptr<Expression>>& get_args() const { return _args; }

    private:
        std::shared_ptr<LValueExpression> _parent;
        std::vector<std::shared_ptr<Expression>> _args;
    };

    class SelfExpression final : public Expression {
    public:
        SelfExpression(std::shared_ptr<SourcePosition> position)
            : Expression(position, nSelfExpression) {}
    };

    class FunctionParameter final : public ASTNode {
    public:
        FunctionParameter(std::shared_ptr<SourcePosition> position, const std::string& identifier,
            std::shared_ptr<Expression> default_expr)
            : ASTNode(position, nFunctionParameter), 
            _identifier(identifier),
            _default_expr(default_expr) {}

        const std::string& get_identifier() const { return _identifier; }
        const std::shared_ptr<Expression>& get_default_expr() const { return _default_expr; }
        bool has_default() const { return _default_expr != nullptr; }

    private:
        std::string _identifier;
        std::shared_ptr<Expression> _default_expr;
    };

    class KeyValuePair final : public ASTNode {
    public:
        KeyValuePair(std::shared_ptr<SourcePosition> position, std::shared_ptr<Expression> key, 
            std::shared_ptr<Expression> value)
            : ASTNode(position, nKeyValuePair),
            _key(key),
            _value(value) {}

        const std::shared_ptr<Expression>& get_key() const { return _key; }
        const std::shared_ptr<Expression>& get_value() const { return _value; }

        private:
            std::shared_ptr<Expression> _key;
            std::shared_ptr<Expression> _value;
    };

    class ASTVisitor {
    protected:
        void Visit(const std::shared_ptr<ASTNode>& node) {
            switch (node->get_type()) {
#define X(NodeType) \
            case ASTNode::n##NodeType:  \
                Visit##NodeType(std::dynamic_pointer_cast<NodeType>(node));    \
                break;
            ALL_NODES
#undef X
            }
        }

#define X(NodeType) virtual void Visit##NodeType(const std::shared_ptr<NodeType>& node) = 0;
        ALL_NODES
#undef X
    };

} // namespace emerald

#endif // _EMERALD_AST_H
