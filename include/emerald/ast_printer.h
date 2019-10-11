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

#define X(NodeType) void Visit##NodeType(const std::shared_ptr<NodeType>& node) override;
        ALL_NODES
#undef X

        void start_indentation_block(const std::string& name);
        void end_indentation_block();

        std::string indent();
    };

} // namespace emerald

#endif // _EMERALD_AST_PRINTER_H
