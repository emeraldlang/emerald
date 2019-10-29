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
#include <sstream>

#include "boost/archive/binary_oarchive.hpp"
#include "boost/archive/binary_iarchive.hpp"
#include "boost/serialization/shared_ptr.hpp"
#include "boost/serialization/vector.hpp"
#include "boost/serialization/unordered_map.hpp"

#include "emerald/check.h"
#include "emerald/code.h"

#define SPACES 4

#define WRITE_OP(op_code) write((Instruction){ .op = op_code })
#define WRITE_OP_WARGS(op_code, arguments...) write((Instruction){ .op = op_code, .args = arguments })

namespace emerald {

    Code::Code()
        : _id(0) {
        _globals = std::make_shared<std::vector<std::string>>();
    }

    Code::Code(const std::filesystem::path& path) {
        std::ifstream ifs(path, std::ios::binary);
        boost::archive::binary_iarchive archive(ifs);
        archive >> *this;
    }

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
            _instructions.at(rewrite_pos).set_arg(0, entry.pos);
        }
    }

    void Code::write_jmp(size_t label) {
        WRITE_OP_WARGS(OpCode::jmp, { get_label_offset(label) });
    }

    void Code::write_jmp_true(size_t label) {
        WRITE_OP_WARGS(OpCode::jmp_true, { get_label_offset(label) });
    }

    void Code::write_jmp_true_or_pop(size_t label) {
        WRITE_OP_WARGS(OpCode::jmp_true_or_pop, { get_label_offset(label) });
    }

    void Code::write_jmp_false(size_t label) {
        WRITE_OP_WARGS(OpCode::jmp_false, { get_label_offset(label) });
    }

    void Code::write_jmp_false_or_pop(size_t label) {
        WRITE_OP_WARGS(OpCode::jmp_false_or_pop, { get_label_offset(label) });
    }

    void Code::write_jmp_data(size_t label) {
        WRITE_OP_WARGS(OpCode::jmp_data, { get_label_offset(label) });
    }

    void Code::write_neg() {
        WRITE_OP(OpCode::neg);
    }

    void Code::write_log_neg() {
        WRITE_OP(OpCode::log_neg);
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

    void Code::write_iadd() {
        WRITE_OP(OpCode::iadd);
    }

    void Code::write_isub() {
        WRITE_OP(OpCode::isub);
    }

    void Code::write_imul() {
        WRITE_OP(OpCode::imul);
    }

    void Code::write_idiv() {
        WRITE_OP(OpCode::idiv);
    }

    void Code::write_imod() {
        WRITE_OP(OpCode::imod);
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

    void Code::write_call(bool receiver, size_t num_args) {
        WRITE_OP_WARGS(OpCode::call, { receiver, num_args });
    }

    void Code::write_ret() {
        WRITE_OP(OpCode::ret);
    }

    void Code::write_new_obj(bool explicit_parent, size_t num_props) {
        WRITE_OP_WARGS(OpCode::new_obj, { explicit_parent, num_props });
    }

    void Code::write_init(size_t num_args) {
        WRITE_OP_WARGS(OpCode::init, { num_args });
    }

    std::shared_ptr<Code> Code::write_new_func(const std::string& label) {
        CHECK_THROW_LOGIC_ERROR(!label.empty(), "cannot have empty label");

        size_t id = _functions.size();
        std::shared_ptr<Code> code(new Code(label, id, _globals));
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

    void Code::write_new_arr(size_t num_elems) {
        WRITE_OP_WARGS(OpCode::new_arr, { num_elems });
    }

    void Code::write_null() {
        WRITE_OP(OpCode::null);
    }

    void Code::write_get_prop(bool push_self_back) {
        WRITE_OP_WARGS(OpCode::get_prop, { push_self_back });
    }

    void Code::write_set_prop(bool push_self_back) {
        WRITE_OP_WARGS(OpCode::set_prop, { push_self_back });
    }

    void Code::write_self() {
        WRITE_OP(OpCode::self);
    }

    void Code::write_enter_try(size_t label) {
        WRITE_OP_WARGS(OpCode::enter_try, { get_label_offset(label) });
    }

    void Code::write_exit_try(size_t label) {
        WRITE_OP_WARGS(OpCode::exit_try, { get_label_offset(label) });
    }

    void Code::write_throw_exc() {
        WRITE_OP(OpCode::throw_exc);
    }

    void Code::write_get_iter() {
        WRITE_OP(OpCode::get_iter);
    }

    void Code::write_iter_cur() {
        WRITE_OP(OpCode::iter_cur);
    }

    void Code::write_iter_done() {
        WRITE_OP(OpCode::iter_done);
    }

    void Code::write_iter_next() {
        WRITE_OP(OpCode::iter_next);
    }

    void Code::write_ldgbl(const std::string& name) {
        size_t i;
        CHECK_THROW_LOGIC_ERROR(get_global_id(name, i),
            "no such global: " + name);
        WRITE_OP_WARGS(OpCode::ldgbl, { i });
    }
    
    void Code::write_stgbl(const std::string& name) {
        size_t i;
        if (!get_global_id(name, i)) {
            i = _globals->size();
            _globals->push_back(name);
        }
        WRITE_OP_WARGS(OpCode::stgbl, { i });
    }

    void Code::write_ldloc(const std::string& name) {
        size_t i;
        CHECK_THROW_LOGIC_ERROR(get_local_id(name, i),
            "no such local: " + name);
        WRITE_OP_WARGS(OpCode::ldloc, { i });
    }

    void Code::write_stloc(const std::string& name) {
        size_t i;
        if (!get_local_id(name, i)) {
            i = _locals.size();
            _locals.push_back(name);
        }
        WRITE_OP_WARGS(OpCode::stloc, { i });
    }

    void Code::write_print() {
        WRITE_OP(OpCode::print);
    }

    size_t Code::write_import(const std::string& name) {
        size_t id = _import_names.size();
        _import_names.push_back(name);

        WRITE_OP_WARGS(OpCode::import, { id });

        return id;
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

    bool Code::is_local_name(const std::string& name) {
        return std::find(_locals.begin(), _locals.end(), name) != _locals.end();
    }

    const std::string& Code::get_local_name(size_t id) const {
        return _locals.at(id);
    }

    const std::vector<std::string>& Code::get_local_names() const {
        return _locals;
    }

    size_t Code::get_num_locals() const {
        return _locals.size();
    }

    bool Code::is_global_name(const std::string& name) {
        return std::find(_globals->begin(), _globals->end(), name) != _globals->end();
    }

    const std::string& Code::get_global_name(size_t id) const {
        return _globals->at(id);
    }

    std::shared_ptr<const std::vector<std::string>> Code::get_global_names() const {
        return _globals;
    }

    size_t Code::get_num_globals() const {
        return _globals->size();
    }

    const std::vector<std::string>& Code::get_import_names() const {
        return _import_names;
    }

    const std::string& Code::get_import_name(size_t id) const {
        return _import_names.at(id);
    }

    std::string Code::to_binary() const {
        std::ostringstream oss;;
        boost::archive::binary_oarchive archive(oss);
        archive << *this;

        return oss.str();
    }

    std::string Code::to_string() const {
        return to_string(0);
    }

    void Code::write_to_file(const std::filesystem::path& path) {
        std::ofstream ofs(path, std::ios::binary);
        boost::archive::binary_oarchive archive(ofs);
        archive << *this;
    }

    void Code::write_to_file_pretty(const std::filesystem::path& path) {
        std::ofstream ofs(path);
        ofs << to_string();
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

    Code::Code(
        const std::string& label,
        size_t id,
        std::shared_ptr<std::vector<std::string>> globals)
        : _label(label),
        _id(id),
        _globals(globals) {}

    void Code::write(const Instruction& instr) { 
        _instructions.push_back(instr); 
    }

    void Code::rewrite(size_t i, const Instruction& instr) {
        _instructions.at(i) = instr;
    }

    std::string Code::to_string(size_t depth) const {
        std::ostringstream oss;
        if (depth > 0 && !_label.empty()) {
            oss << std::string((depth - 1) * SPACES, ' ') << _label << '(' << _id << "):" << std::endl;
        }

        for (size_t i = 0; i < _instructions.size(); i++) {
            if (i > 0) oss << std::endl;
            oss << i << ": " << std::string(depth * SPACES, ' ') << _instructions[i];
        }

        for (size_t i = 0; i < _functions.size(); i++) {
            oss << std::endl << _functions[i]->to_string(depth + 1);
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

    bool Code::get_global_id(const std::string& name, size_t& i) {
        i = 0;
        for (; i < _globals->size(); i++) {
            if ((*_globals)[i] == name) break;
        }

        return i < _globals->size();
    }

    bool Code::get_local_id(const std::string& name, size_t& i) {
        i = 0;
        for (; i < _locals.size(); i++) {
            if (_locals[i] == name) break;
        }

        return i < _locals.size();
    }

    Code::Instruction::Instruction()
        : _op(OpCode::nop) {}

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
