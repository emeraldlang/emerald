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

#ifndef _EMERALD_COMPILER_H
#define _EMERALD_COMPILER_H

#include <memory>
#include <stack>
#include <vector>

#include "emerald/ast.h"
#include "emerald/code.h"
#include "emerald/reporter.h"

namespace emerald {

    class Compiler : public ASTVisitor {
    public:
        static std::shared_ptr<Code> compile(
            const std::vector<std::shared_ptr<Statement>>& statements,
            std::shared_ptr<Reporter> reporter);

    private:
        struct LoopLabels {
            LoopLabels(size_t condition, size_t _beginning, size_t _end)
                : condition(condition), 
                beginning(_beginning),
                end(_end) {}

            size_t condition;
            size_t beginning;
            size_t end;
        };

        Compiler(std::shared_ptr<Reporter> reporter);

        std::shared_ptr<Reporter> _reporter;
        std::shared_ptr<Code> _code;
        std::stack<std::shared_ptr<Code>> _code_stack;
        std::stack<LoopLabels> _loop_stack;

#define X(NodeType) void Visit##NodeType(const std::shared_ptr<NodeType>& node) override;
        ALL_NODES
#undef X

        void VisitPropertyLoad(const std::shared_ptr<Property>& property, bool push_self_back = false);
        void VisitPropertyStore(const std::shared_ptr<Property>& property, const std::shared_ptr<Expression>& val, bool push_self_back = false);

        void VisitIdentifierLoad(const std::shared_ptr<Identifier>& identifier);
        void VisitIdentifierStore(const std::shared_ptr<Identifier>& identifier, const std::shared_ptr<Expression>& val);

        void push_new_func(const std::string& label);
        void pop_func();
        bool is_top_level();
        bool is_code_top_level(const std::shared_ptr<Code>& code);

        std::shared_ptr<Code> code() {
            if (_code_stack.size()) {
                return _code_stack.top();
            }

            return _code;
        }
        
        void write_fs_load(const std::shared_ptr<ForStatement>& for_statement);
        void write_fs_condition(const std::shared_ptr<ForStatement>& for_statement);

        void write_st(const std::shared_ptr<Code>& code, const std::string& identifier);
        void write_st(const std::string& identifier);

        void write_comp_assign(const std::shared_ptr<Token>& op);
    };

} // namespace emerald

#endif // _EMERALD_COMPILER_H
