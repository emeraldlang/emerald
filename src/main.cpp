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
#include <vector>

#include "CLI/CLI.hpp"

#include "emerald/ast_printer.h"
#include "emerald/colors.h"
#include "emerald/compiler.h"
#include "emerald/interpreter.h"
#include "emerald/modules/init.h"
#include "emerald/parser.h"
#include "emerald/reporter.h"
#include "emerald/source.h"
#include "emerald/strutils.h"

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
        std::filesystem::path path(bytecode_source_file);
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
            code->write_to_file_pretty(path.replace_extension(".emb"));
        } else {
            std::cout << code->to_string() << std::endl;
        }
    });

    CLI::App* compile = app.add_subcommand(
        "compile",
        "compiles an emerald source file into bytecode.");

    std::vector<std::filesystem::path> compile_source_files;
    compile->add_option("source_files", compile_source_files, "specifies the emerald source files")->required();

    std::filesystem::path compile_output;
    compile->add_option("-o,--output", compile_output, "specifies the output directory");

    compile->callback([&]() {
        std::shared_ptr<emerald::Reporter> reporter = std::make_shared<emerald::Reporter>();
        for (const std::filesystem::path& path : compile_source_files) {
            std::vector<std::shared_ptr<emerald::Statement>> statements = emerald::Parser::parse(
                emerald::Source::from_file(path),
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

            std::filesystem::path output_path;
            if (compile_output.empty()) {
                output_path = std::filesystem::path(path).replace_extension(".emc");
            } else {
                output_path = compile_output / path.filename().replace_extension(".emc");
            }

            code->write_to_file(output_path);
        }
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

    std::string run_module_name;
    run->add_option("module_name", run_module_name, "specifies the emerald module to execute")->required();

    run->callback([&]() {
        emerald::modules::add_module_inits_to_registry();
        emerald::Interpreter::execute_module(run_module_name);
    });

    CLI11_PARSE(app, argc, argv);
    return 0;
}
