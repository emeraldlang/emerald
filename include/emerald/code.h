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

#ifndef _EMERALD_CODE_H
#define _EMERALD_CODE_H

#include <cstdint>
#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "emerald/opcode.h"

namespace emerald {

    class Code {
    public:
        Code();

        class Instruction {
        public:
            Instruction(OpCode::Value op);
            Instruction(OpCode::Value op, std::initializer_list<uint64_t> args);

            OpCode::Value get_op() const;

            size_t get_arg_count() const;
            const std::vector<uint64_t>& get_args() const;
            uint64_t get_arg(size_t i) const;
            void set_arg(size_t i, uint64_t val);

            std::string to_string() const;

        private:
            OpCode::Value _op;
            std::vector<uint64_t> _args;
        };

        const std::string& get_label() const;
        size_t get_id() const;

        size_t get_num_instructions() const;

        void write_nop();

        size_t create_label();
        void bind_label(size_t label);

        void write_jmp(size_t label);
        void write_jmp_true(size_t label);
        void write_jmp_false(size_t label);

        void write_neg();
        void write_add();
        void write_sub();
        void write_mul();
        void write_div();
        void write_mod();
        void write_prefix_inc();
        void write_prefix_dec();
        void write_postfix_inc();
        void write_postfix_dec();

        void write_eq();
        void write_neq();
        void write_lt();
        void write_gt();
        void write_lte();
        void write_gte();

        void write_bit_not();
        void write_bit_or();
        void write_bit_xor();
        void write_bit_and();
        void write_bit_shl();
        void write_bit_shr();

        void write_str();
        void write_boolean();

        void write_call(size_t num_args);
        void write_ret();

        void write_new_obj(bool explicit_parent, size_t num_props);
        void write_new_obj_and_init(bool explicit_parent, size_t num_props, size_t num_args);
        std::shared_ptr<Code> write_new_func(const std::string& label);
        size_t write_new_num(double val);
        size_t write_new_str(const std::string& val);
        void write_new_boolean(bool val);
        void write_new_arr(size_t num_elems);
        void write_get_prop();
        void write_has_prop();
        void write_set_prop();
        void write_get_parent();

        void write_ldloc(const std::string& name);
        void write_stloc(const std::string& name);

        void write_print();

        std::shared_ptr<const Code> get_func(const std::string& label) const;
        std::shared_ptr<Code> get_func(const std::string& label);
        std::shared_ptr<const Code> get_func(size_t id) const;
        std::shared_ptr<Code> get_func(size_t id);

        size_t get_func_index(const std::string& label) const;

        double get_num_constant(size_t id) const;
        const std::string& get_str_constant(size_t id) const;

        const std::string& get_local_name(size_t id) const;
        const std::vector<std::string>& get_local_names() const;
        size_t get_num_locals() const;

        std::string stringify() const;

        void write_to_file(const std::filesystem::path& path);
        void write_to_file_pretty(const std::filesystem::path& path);

        const Instruction& operator[](size_t i) const;

        std::vector<Instruction>::iterator begin();
        std::vector<Instruction>::iterator end();

        std::vector<Instruction>::const_iterator begin() const;
        std::vector<Instruction>::const_iterator end() const;

    private:
        struct LabelEntry {
            size_t pos = 0;
            bool is_bound = false;
            std::vector<size_t> unbound_rewrites;
        };

        std::string _label;
        size_t _id;

        std::vector<Instruction> _instructions;

        std::vector<std::shared_ptr<Code>> _functions;
        std::unordered_map<std::string, size_t> _function_labels;

        std::vector<double> _num_constants;
        std::vector<std::string> _str_constants;

        std::vector<LabelEntry> _labels;

        std::vector<std::string> _locals;

        Code(const std::string& label, size_t id);

        void write(const Instruction& instr);
        void rewrite(size_t i, const Instruction& instr);

        std::string stringify(size_t depth) const;

        size_t get_label_offset(size_t label);

        size_t get_local_id(const std::string& name);
    };

    std::ostream& operator<<(std::ostream& os, const Code::Instruction& instr);

} // namespace emerald

#endif // _EMERALD_CODE_H
