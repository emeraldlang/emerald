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
        Code(const std::filesystem::path& path);

        class Instruction {
        public:
            Instruction();
            Instruction(OpCode::Value op);
            Instruction(OpCode::Value op, std::initializer_list<uint64_t> args);

            OpCode::Value get_op() const;

            size_t get_arg_count() const;
            const std::vector<uint64_t>& get_args() const;
            uint64_t get_arg(size_t i) const;
            void set_arg(size_t i, uint64_t val);

            std::string to_string() const;

            template<class Archive>
            void serialize(Archive& archive, const unsigned int);

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
        void write_jmp_data(size_t label);

        void write_neg();
        void write_add();
        void write_sub();
        void write_mul();
        void write_div();
        void write_mod();
        
        void write_iadd();
        void write_isub();
        void write_imul();
        void write_idiv();
        void write_imod();

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
        void write_init(size_t num_args);
        std::shared_ptr<Code> write_new_func(const std::string& label);
        size_t write_new_num(double val);
        size_t write_new_str(const std::string& val);
        void write_new_boolean(bool val);
        void write_new_arr(size_t num_elems);
        void write_null();
        void write_get_prop(bool push_self_back = false);
        void write_has_prop(bool push_self_back = false);
        void write_set_prop(bool push_self_back = false);
        void write_get_parent();

        void write_ldgbl(const std::string& name);
        void write_stgbl(const std::string& name);
        void write_ldloc(const std::string& name);
        void write_stloc(const std::string& name);

        void write_print();
        size_t write_import(const std::string& name);

        std::shared_ptr<const Code> get_func(const std::string& label) const;
        std::shared_ptr<Code> get_func(const std::string& label);
        std::shared_ptr<const Code> get_func(size_t id) const;
        std::shared_ptr<Code> get_func(size_t id);

        size_t get_func_index(const std::string& label) const;

        double get_num_constant(size_t id) const;
        const std::string& get_str_constant(size_t id) const;

        bool is_local_name(const std::string& name);
        const std::string& get_local_name(size_t id) const;
        const std::vector<std::string>& get_local_names() const;
        size_t get_num_locals() const;

        bool is_global_name(const std::string& name);
        const std::string& get_global_name(size_t id) const;
        std::shared_ptr<const std::vector<std::string>> get_global_names() const;
        size_t get_num_globals() const;

        const std::vector<std::string>& get_import_names() const;
        const std::string& get_import_name(size_t id) const;

        std::string to_binary() const;
        std::string to_string() const;

        void write_to_file(const std::filesystem::path& path);
        void write_to_file_pretty(const std::filesystem::path& path);

        const Instruction& operator[](size_t i) const;

        std::vector<Instruction>::iterator begin();
        std::vector<Instruction>::iterator end();

        std::vector<Instruction>::const_iterator begin() const;
        std::vector<Instruction>::const_iterator end() const;

        template<class Archive>
        void serialize(Archive& archive, const unsigned int);

    private:
        struct LabelEntry {
            size_t pos = 0;
            bool is_bound = false;
            std::vector<size_t> unbound_rewrites;

            template<class Archive>
            void serialize(Archive& archive, const unsigned int);
        };

        std::string _label;
        size_t _id;

        std::vector<Instruction> _instructions;

        std::vector<std::shared_ptr<Code>> _functions;
        std::unordered_map<std::string, size_t> _function_labels;

        std::vector<double> _num_constants;
        std::vector<std::string> _str_constants;

        std::vector<std::string> _import_names;

        std::vector<LabelEntry> _labels;

        std::vector<std::string> _locals;
        std::shared_ptr<std::vector<std::string>> _globals;

        Code(  
            const std::string& label,
            size_t id,
            std::shared_ptr<std::vector<std::string>> globals);

        void write(const Instruction& instr);
        void rewrite(size_t i, const Instruction& instr);

        std::string to_string(size_t depth) const;

        size_t get_label_offset(size_t label);

        bool get_global_id(const std::string& name, size_t& i);
        bool get_local_id(const std::string& name, size_t& i);
    };

    std::ostream& operator<<(std::ostream& os, const Code::Instruction& instr);

    template<class Archive>
    void Code::serialize(Archive& archive, const unsigned int) {
        archive & _label;
        archive & _id;
        archive & _instructions;
        archive & _functions;
        archive & _function_labels;
        archive & _num_constants;
        archive & _str_constants;
        archive & _labels;
        archive & _locals;
        archive & _globals;
        archive & _import_names;
    }

    template<class Archive>
    void Code::Instruction::serialize(Archive& archive, const unsigned int) {
        archive & _op;
        archive & _args;
    }

    template<class Archive>
    void Code::LabelEntry::serialize(Archive& archive, const unsigned int) {
        archive & pos;
        archive & is_bound;
        archive & unbound_rewrites;
    }

} // namespace emerald

#endif // _EMERALD_CODE_H
