#pragma once

#include <alaio/vm/interpret_visitor.hpp>
#include <alaio/vm/opcodes.hpp>

#include <iostream>

#define DBG_VISIT(name, code)                                                                                          \
   void operator()(const ALA_VM_OPCODE_T(name)& op) {                                                                  \
      std::cout << "Found " << #name << " at " << get_context().get_pc() << "\n";                                      \
      interpret_visitor<ExecutionCTX>::operator()(op);                                                                 \
      get_context().print_stack();                                                                                     \
   }

#define DBG2_VISIT(name, code)                                                                                         \
   void operator()(const ALA_VM_OPCODE_T(name)& op) { std::cout << "Found " << #name << "\n"; }

namespace alaio { namespace vm {

template <typename ExecutionCTX>
struct debug_visitor : public interpret_visitor<ExecutionCTX> {
   using interpret_visitor<ExecutionCTX>::operator();
   debug_visitor(ExecutionCTX& ctx) : interpret_visitor<ExecutionCTX>(ctx) {}
   ExecutionCTX& get_context() { return interpret_visitor<ExecutionCTX>::get_context(); }
   ALA_VM_CONTROL_FLOW_OPS(DBG_VISIT)
   ALA_VM_BR_TABLE_OP(DBG_VISIT)
   ALA_VM_RETURN_OP(DBG_VISIT)
   ALA_VM_CALL_OPS(DBG_VISIT)
   ALA_VM_CALL_IMM_OPS(DBG_VISIT)
   ALA_VM_PARAMETRIC_OPS(DBG_VISIT)
   ALA_VM_VARIABLE_ACCESS_OPS(DBG_VISIT)
   ALA_VM_MEMORY_OPS(DBG_VISIT)
   ALA_VM_I32_CONSTANT_OPS(DBG_VISIT)
   ALA_VM_I64_CONSTANT_OPS(DBG_VISIT)
   ALA_VM_F32_CONSTANT_OPS(DBG_VISIT)
   ALA_VM_F64_CONSTANT_OPS(DBG_VISIT)
   ALA_VM_COMPARISON_OPS(DBG_VISIT)
   ALA_VM_NUMERIC_OPS(DBG_VISIT)
   ALA_VM_CONVERSION_OPS(DBG_VISIT)
   ALA_VM_EXIT_OP(DBG_VISIT)
   ALA_VM_ERROR_OPS(DBG_VISIT)
};

struct debug_visitor2 {
   ALA_VM_CONTROL_FLOW_OPS(DBG2_VISIT)
   ALA_VM_BR_TABLE_OP(DBG2_VISIT)
   ALA_VM_RETURN_OP(DBG2_VISIT)
   ALA_VM_CALL_OPS(DBG2_VISIT)
   ALA_VM_CALL_IMM_OPS(DBG2_VISIT)
   ALA_VM_PARAMETRIC_OPS(DBG2_VISIT)
   ALA_VM_VARIABLE_ACCESS_OPS(DBG2_VISIT)
   ALA_VM_MEMORY_OPS(DBG2_VISIT)
   ALA_VM_I32_CONSTANT_OPS(DBG2_VISIT)
   ALA_VM_I64_CONSTANT_OPS(DBG2_VISIT)
   ALA_VM_F32_CONSTANT_OPS(DBG2_VISIT)
   ALA_VM_F64_CONSTANT_OPS(DBG2_VISIT)
   ALA_VM_COMPARISON_OPS(DBG2_VISIT)
   ALA_VM_NUMERIC_OPS(DBG2_VISIT)
   ALA_VM_CONVERSION_OPS(DBG2_VISIT)
   ALA_VM_EXIT_OP(DBG2_VISIT)
   ALA_VM_ERROR_OPS(DBG2_VISIT)
};
#undef DBG_VISIT
#undef DBG2_VISIT

#undef DBG_VISIT
#undef DBG2_VISIT

}} // ns alaio::wasm_backend
