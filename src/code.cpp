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

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iterator>
#include <sstream>

#include "emerald/check.h"
#include "emerald/code.h"

#define SPACES 4

#define WRITE_OP(op_code) write((Instruction){ .op = op_code })
#define WRITE_OP_WARGS(op_code, arguments...) write((Instruction){ .op = op_code, .args = arguments })

namespace emerald {

    Code::Code()
        : _id(0) {}

    const std::string& Code::get_label() const {
        return _label;
    }

    size_t Code::get_id() const {
        return _id;
    }

    size_t Code::get_num_instructions() const { 
        return _instructions.size(); 
    }

    void Code::write_nop() {
        WRITE_OP(OpCode::nop);
    }

    size_t Code::create_label() {
        size_t id = _labels.size();
        _labels.push_back(LabelEntry());
        return id;
    }
    
    void Code::bind_label(size_t label) {
        LabelEntry& entry = _labels.at(label);
        CHECK_THROW_LOGIC_ERROR(!entry.is_bound,
            "label is already bound to a position");

        entry.pos = _instructions.size();
        for (size_t rewrite_pos : entry.unbound_rewrites) {
            _instructions.at(rewrite_pos).set_arg(0, entry.pos - rewrite_pos);
        }
    }

    void Code::write_jmp(size_t label) {
        WRITE_OP_WARGS(OpCode::jmp, { get_label_offset(label) });
    }

    void Code::write_jmp_true(size_t label) {
        WRITE_OP_WARGS(OpCode::jmp_true, { get_label_offset(label) });
    }

    void Code::write_jmp_false(size_t label) {
        WRITE_OP_WARGS(OpCode::jmp_false, { get_label_offset(label) });
    }

    void Code::write_neg() {
        WRITE_OP(OpCode::neg);
    }

    void Code::write_add() {
        WRITE_OP(OpCode::add);
    }

    void Code::write_sub() {
        WRITE_OP(OpCode::sub);
    }

    void Code::write_mul() {
        WRITE_OP(OpCode::mul);
    }

    void Code::write_div() {
        WRITE_OP(OpCode::div);
    }

    void Code::write_mod() {
        WRITE_OP(OpCode::mod);
    }

    void Code::write_prefix_inc() {
        WRITE_OP(OpCode::prefix_inc);
    }

    void Code::write_prefix_dec() {
        WRITE_OP(OpCode::prefix_dec);
    }

    void Code::write_postfix_inc() {
        WRITE_OP(OpCode::postfix_inc);
    }

    void Code::write_postfix_dec() {
        WRITE_OP(OpCode::postfix_dec);
    }

    void Code::write_eq() {
        WRITE_OP(OpCode::eq);
    }

    void Code::write_neq() {
        WRITE_OP(OpCode::neq);
    }

    void Code::write_lt() {
        WRITE_OP(OpCode::lt);
    }

    void Code::write_gt() {
        WRITE_OP(OpCode::gt);
    }

    void Code::write_lte() {
        WRITE_OP(OpCode::lte);
    }

    void Code::write_gte() {
        WRITE_OP(OpCode::gte);
    }

    void Code::write_bit_not() {
        WRITE_OP(OpCode::bit_not);
    }

    void Code::write_bit_or() {
        WRITE_OP(OpCode::bit_or);
    }

    void Code::write_bit_xor() {
        WRITE_OP(OpCode::bit_xor);
    }

    void Code::write_bit_and() {
        WRITE_OP(OpCode::bit_and);
    }

    void Code::write_bit_shl() {
        WRITE_OP(OpCode::bit_shl);
    }
    
    void Code::write_bit_shr() {
        WRITE_OP(OpCode::bit_shr);
    }

    void Code::write_str() {
        WRITE_OP(OpCode::str);
    }

    void Code::write_boolean() {
        WRITE_OP(OpCode::boolean);
    }

    void Code::write_call(size_t num_args) {
        WRITE_OP_WARGS(OpCode::call, { num_args });
    }

    void Code::write_ret() {
        WRITE_OP(OpCode::ret);
    }

    std::shared_ptr<Code> Code::write_new_func(const std::string& label) {
        CHECK_THROW_LOGIC_ERROR(!label.empty(), "cannot have empty label");

        size_t id = _functions.size();
        std::shared_ptr<Code> code(new Code(label, id));
        _functions.push_back(code);
        _function_labels[label] = id;

        WRITE_OP_WARGS(OpCode::new_func, { id });

        return code;
    }

    size_t Code::write_new_num(double val) {
        size_t id = _num_constants.size();
        _num_constants.push_back(val);

        WRITE_OP_WARGS(OpCode::new_num, { id });

        return id;
    }

    size_t Code::write_new_str(const std::string& val) {
        size_t id = _str_constants.size();
        _str_constants.push_back(val);

        WRITE_OP_WARGS(OpCode::new_str, { id });

        return id;
    }

    void Code::write_new_boolean(bool val) {
        WRITE_OP_WARGS(OpCode::new_boolean, { val });
    }
    
    void Code::write_clone(bool explicit_parent) {
        WRITE_OP_WARGS(OpCode::clone, { explicit_parent });
    }
    
    void Code::write_init(size_t num_args) {
        WRITE_OP_WARGS(OpCode::init, { num_args });
    }

    void Code::write_get_prop() {
        WRITE_OP(OpCode::get_prop);
    }

    void Code::write_has_prop() {
        WRITE_OP(OpCode::has_prop);
    }

    void Code::write_set_prop() {
        WRITE_OP(OpCode::set_prop);
    }

    void Code::write_ldloc(size_t loc_indx) {
        WRITE_OP_WARGS(OpCode::ldloc, { loc_indx });
    }

    void Code::write_stloc(size_t loc_indx) {
        WRITE_OP_WARGS(OpCode::stloc, { loc_indx });
    }

    void Code::write_print() {
        WRITE_OP(OpCode::print);
    }

    void Code::write_allocate_locals(size_t locals) {
        WRITE_OP_WARGS(OpCode::allocate_locals, { locals });
    }

    std::shared_ptr<const Code> Code::get_func(const std::string& label) const {
        return _functions[_function_labels.at(label)];
    }

    std::shared_ptr<Code> Code::get_func(const std::string& label) {
        return _functions[_function_labels.at(label)];
    }

    std::shared_ptr<const Code> Code::get_func(size_t id) const {
        return _functions.at(id);
    }

    std::shared_ptr<Code> Code::get_func(size_t id) {
        return _functions.at(id);
    }

    size_t Code::get_func_index(const std::string& label) const {
        return _function_labels.at(label);
    }

    double Code::get_num_constant(size_t id) const {
        return _num_constants.at(id);
    }
    
    const std::string& Code::get_str_constant(size_t id) const {
        return _str_constants.at(id);
    }

    std::string Code::stringify() const {
        return stringify(0);
    }

    void Code::write_to_file(const std::filesystem::path&) {
        // todo(zvp): design file structure, headers with metadata
    }

    void Code::write_to_file_pretty(const std::filesystem::path&) {
        // todo(zvp): implement me
    }

    const Code::Instruction& Code::operator[](size_t i) const {
        return _instructions.at(i);
    }

    std::vector<Code::Instruction>::iterator Code::begin() { 
        return _instructions.begin();  
    }

    std::vector<Code::Instruction>::iterator Code::end() { 
        return _instructions.end(); 
    }

    std::vector<Code::Instruction>::const_iterator Code::begin() const { 
        return _instructions.begin(); 
    }

    std::vector<Code::Instruction>::const_iterator Code::end() const { 
        return _instructions.end(); 
    }

    Code::Code(const std::string& label, size_t id)
        : _label(label),
        _id(id) {}

    void Code::write(const Instruction& instr) { 
        _instructions.push_back(instr); 
    }

    void Code::rewrite(size_t i, const Instruction& instr) {
        _instructions.at(i) = instr;
    }

    std::string Code::stringify(size_t depth) const {
        std::ostringstream oss;
        bool include_label = depth > 0 && !_label.empty();
        if (include_label) {
            oss << _label << '(' << _id << "):" << std::endl;
        }

        for (size_t i = 0; i < _instructions.size(); i++) {
            if (i > 0) oss << std::endl;
            if (include_label) oss << std::setw(SPACES * depth) << std::setfill(' ');
            oss << _instructions[i];
        }

        for (size_t i = 0; i < _functions.size(); i++) {
            oss << std::endl << _functions[i]->stringify(i + 1);
        }

        return oss.str();
    }

    size_t Code::get_label_offset(size_t label) {
        LabelEntry& entry = _labels.at(label);
        if (!entry.is_bound) {
            entry.unbound_rewrites.push_back(_instructions.size());
        }

        return entry.pos;
    }

    Code::Instruction::Instruction(OpCode::Value op)
        : _op(op) {
        CHECK_THROW_INVALID_ARGUMENT(OpCode::get_arg_count(_op) == 0,
            "invalid number of arguments passed");
    }

    Code::Instruction::Instruction(OpCode::Value op, std::initializer_list<uint64_t> args)
        : _op(op),
        _args(args) {
        CHECK_THROW_INVALID_ARGUMENT(_args.size() == OpCode::get_arg_count(_op),
            "invalid number of arguments passed");
    }

    OpCode::Value Code::Instruction::get_op() const { 
        return _op; 
    }

    size_t Code::Instruction::get_arg_count() const { 
        return _args.size(); 
    }

    const std::vector<uint64_t>& Code::Instruction::get_args() const { 
        return _args; 
    }

    uint64_t Code::Instruction::get_arg(size_t i) const { 
        return _args.at(i); 
    }

    void Code::Instruction::set_arg(size_t i, uint64_t val) {
        _args.at(i) = val;
    }

    std::string Code::Instruction::to_string() const {
        std::ostringstream oss;

        oss << OpCode::get_string(_op);

        size_t size = _args.size();
        for (size_t i = 0; i < size; i++) {
            if (i == 0) oss << ' ';
            else oss << ',';
            oss << _args[i];
        }
        
        return oss.str();
    }

    std::ostream& operator<<(std::ostream& os, const Code::Instruction& instr) {
        os << instr.to_string();
        return os;
    }

} // namespace emerald
