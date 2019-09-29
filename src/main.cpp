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
#include "emerald/colors.h"
#include "emerald/compiler.h"
#include "emerald/module.h"
#include "emerald/modules/init.h"
#include "emerald/parser.h"
#include "emerald/reporter.h"
#include "emerald/source.h"
#include "emerald/strutils.h"
#include "emerald/vm.h"

int main(int argc, char** argv) {
    CLI::App app;
    app.require_subcommand();

    CLI::App* ast = app.add_subcommand(
        "ast", 
        "prints the abstract syntax tree of a emerald source file.");

    std::string ast_module_name;
    ast->add_option("module_name", ast_module_name, "specifies the emerald module")->required();

    ast->callback([&]() {
        std::shared_ptr<emerald::Reporter> reporter = std::make_shared<emerald::Reporter>();
        std::vector<std::shared_ptr<emerald::Statement>> statements = emerald::Parser::parse(
            emerald::Source::from_file(ast_module_name),
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

    std::string bytecode_module_name;
    bytecode->add_option("module_name", bytecode_module_name, "specifies the emerald module")->required();

    bool bytecode_save;
    bytecode->add_flag("-s,--save", bytecode_save, "indicates whether the bytecode should be persisted to desk");

    bytecode->callback([&]() {
        std::shared_ptr<emerald::Reporter> reporter = std::make_shared<emerald::Reporter>();

        std::filesystem::path path = emerald::Module::get_path_for_module(bytecode_module_name, ".em");
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

        if (bytecode_save) {
            code->write_to_file_pretty(path.replace_extension(".emb"));
        } else {
            std::cout << code->to_string() << std::endl;
        }
    });

    CLI::App* compile = app.add_subcommand(
        "compile",
        "compiles an emerald source file into bytecode.");

    std::string compile_module_name;
    compile->add_option("module_name", compile_module_name, "specifies the emerald module")->required();

    compile->callback([&]() {
        std::shared_ptr<emerald::Reporter> reporter = std::make_shared<emerald::Reporter>();

        std::filesystem::path path = emerald::Module::get_path_for_module(compile_module_name, ".em");
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

        code->write_to_file(path.replace_extension(".emc"));
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
    run->add_option("module_name", run_module_name, "specifies the emerald module")->required();

    run->callback([&]() {
        emerald::modules::add_module_inits_to_registry();

        emerald::VM vm;

        vm.start();
        vm.create_process(run_module_name);

        while (true) {
            std::string input;
            getline(std::cin, input);

            std::vector<std::string> args = emerald::strutils::split(input, " ");

            std::string cmd = std::move(args[0]);
            args.erase(args.begin());

            if (cmd == "create_process") {
                if (args.size() == 0) {
                    std::cout << emerald::colors::fg_red << "create_process: " << emerald::colors::reset << "expected one argument" << std::endl;
                    continue;
                }

                for (const std::string& module_name : args) {
                    vm.create_process(module_name);
                }
            } else if (cmd == "exit") {
                if (vm.running()) {
                    std::cout << "stopping vm...";
                    vm.stop();
                    std::cout << "done." << std::endl;
                }
                break;
            } else if (cmd == "scale") {
                if (args.size() == 0) {
                    std::cout << emerald::colors::fg_red << "scale: " << emerald::colors::reset << "expected one argument" << std::endl;
                } else {
                    size_t n = std::stoul(args[0]);
                    vm.scale(n);
                }
            } else if (cmd == "start") {
                if (vm.running()) {
                    std::cout << emerald::colors::fg_red << "vm is already running" << emerald::colors::reset << std::endl;
                } else {
                    std::cout << "starting vm...";
                    vm.start();
                    std::cout << "done." << std::endl;
                }
            } else if (cmd == "stop") {
                if (!vm.running()) {
                    std::cout << emerald::colors::fg_red << "vm is already stopped" << emerald::colors::reset << std::endl;
                } else {
                    std::cout << "stopping vm...";
                    vm.stop();
                    std::cout << "done." << std::endl;
                }
            } else {
                std::cout << emerald::colors::fg_red << "unknown cmd: " << emerald::colors::reset << cmd << std::endl;
            }
        }
    });

    CLI11_PARSE(app, argc, argv);
    return 0;
}
