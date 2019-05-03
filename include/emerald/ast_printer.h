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

#ifndef _EMERALD_AST_PRINTER_H
#define _EMERALD_AST_PRINTER_H

#include <memory>
#include <sstream>
#include <vector>

#include "emerald/ast.h"

namespace emerald {

    class ASTPrinter final : public ASTVisitor {
    public:
        static void print(const std::vector<std::shared_ptr<Statement>>& statements);

    private:
        std::ostringstream _oss;
        size_t _indentation;

        ASTPrinter();

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

        void start_indentation_block(const std::string& name);
        void end_indentation_block();

        std::string indent();
    };

} // namespace emerald

#endif // _EMERALD_AST_PRINTER_H
