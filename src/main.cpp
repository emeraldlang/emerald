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

#include <filesystem>
#include <iostream>
#include <memory>

#include "CLI/CLI.hpp"

#include "emerald/ast_printer.h"
#include "emerald/compiler.h"
#include "emerald/modules/init.h"
#include "emerald/parser.h"
#include "emerald/reporter.h"
#include "emerald/source.h"
#include "emerald/vm.h"

int main(int argc, char** argv) {
    CLI::App app;
    app.require_subcommand();

    CLI::App* ast = app.add_subcommand(
        "ast", 
        "prints the abstract syntax tree of a emerald source file.");

    std::string ast_source_file;
    ast->add_option("source_file", ast_source_file, "specifies the emerald source file")->required();

    ast->callback([&]() {
        std::shared_ptr<emerald::Reporter> reporter = std::make_shared<emerald::Reporter>();
        std::vector<std::shared_ptr<emerald::Statement>> statements = emerald::Parser::parse(
            emerald::Source::from_file(ast_source_file),
            reporter);
        if (reporter->has_errors()) {
            reporter->print();
        } else {
            emerald::ASTPrinter::print(statements);
        }
    });

    CLI::App* bytecode = app.add_subcommand(
        "bytecode", 
        "prints the bytecode of a emerald source file.");

    std::string bytecode_source_file;
    bytecode->add_option("source_file", bytecode_source_file, "specifies the emerald source file")->required();

    bool bytecode_save;
    bytecode->add_flag("-s,--save", bytecode_save, "indicates whether the bytecode should be persisted to desk");

    bytecode->callback([&]() {
        std::shared_ptr<emerald::Reporter> reporter = std::make_shared<emerald::Reporter>();

        std::vector<std::shared_ptr<emerald::Statement>> statements = emerald::Parser::parse(
            emerald::Source::from_file(bytecode_source_file),
            reporter);
        if (reporter->has_errors()) {
            reporter->print();
            return;
        }

        std::shared_ptr<emerald::Code> code = emerald::Compiler::compile(
            statements,
            reporter);
        if (reporter->has_errors()) {
            reporter->print();
            return;
        }

        if (bytecode_save) {
            code->write_to_file_pretty(
                std::filesystem::path(bytecode_source_file).replace_extension(".emb"));
        } else {
            std::cout << code->to_string() << std::endl;
        }
    });

    CLI::App* compile = app.add_subcommand(
        "compile",
        "compiles an emerald source file into bytecode.");

    std::string compile_source_file;
    compile->add_option("source_file", compile_source_file, "specifies the emerald source file")->required();

    compile->callback([&]() {
        std::shared_ptr<emerald::Reporter> reporter = std::make_shared<emerald::Reporter>();

        std::vector<std::shared_ptr<emerald::Statement>> statements = emerald::Parser::parse(
            emerald::Source::from_file(compile_source_file),
            reporter);
        if (reporter->has_errors()) {
            reporter->print();
            return;
        }

        std::shared_ptr<emerald::Code> code = emerald::Compiler::compile(
            statements,
            reporter);
        if (reporter->has_errors()) {
            reporter->print();
            return;
        }

        code->write_to_file(std::filesystem::path(compile_source_file).replace_extension(".emc"));
    });

    CLI::App* init = app.add_subcommand(
        "init",
        "initializes a new emerald project.");

    init->callback([&]() {
        std::cout
            << "This command will create an emerald build for you." << std::endl
            << "After the YAML file has been created for you can use" << std::endl
            << "`emerald run [-b, --build_config_path = build.yaml]` to" << std::endl
            << "run your program." << std::endl;
    });


    CLI::App* run = app.add_subcommand(
        "run",
        "executes the emerald code.");

    std::string run_source_file;
    run->add_option("source_file", run_source_file, "specifies the emerald source file")->required();

    run->callback([&]() {
        emerald::modules::add_module_inits_to_registry();

        // emerald::VM vm;
        // vm.start();
        // vm.create_process(code);

        // vm.stop();
    });

    CLI11_PARSE(app, argc, argv);
    return 0;
}
