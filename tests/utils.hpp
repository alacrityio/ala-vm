#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>
#include <alaio/vm/allocator.hpp>
#include <alaio/vm/stack_elem.hpp>
#include <alaio/vm/utils.hpp>

struct type_converter32 {
   union {
      uint32_t ui;
      float    f;
   } _data;
   type_converter32(uint32_t n) { _data.ui = n; }
   uint32_t to_ui() const { return _data.ui; }
   float    to_f() const { return _data.f; }
};

struct type_converter64 {
   union {
      uint64_t ui;
      double   f;
   } _data;
   type_converter64(uint64_t n) { _data.ui = n; }
   uint64_t to_ui() const { return _data.ui; }
   double   to_f() const { return _data.f; }
};

// C++20: using std::bit_cast;
template<typename T, typename U>
T bit_cast(const U& u) {
   static_assert(sizeof(T) == sizeof(U), "bitcast requires identical sizes.");
   T result;
   std::memcpy(&result, &u, sizeof(T));
   return result;
}


inline bool check_nan(const std::optional<alaio::vm::operand_stack_elem>& v) {
   return visit(alaio::vm::overloaded{[](alaio::vm::i32_const_t){ return false; },
                                      [](alaio::vm::i64_const_t){ return false; },
                                      [](alaio::vm::f32_const_t f) { return std::isnan(f.data.f); },
                                      [](alaio::vm::f64_const_t f) { return std::isnan(f.data.f); }}, *v);
}

inline alaio::vm::wasm_allocator* get_wasm_allocator() {
   static alaio::vm::wasm_allocator alloc;
   return &alloc;
}

#ifdef __x86_64__
#define BACKEND_TEST_CASE(name, tags) \
  TEMPLATE_TEST_CASE(name, tags, alaio::vm::interpreter, alaio::vm::jit)
#else
#define BACKEND_TEST_CASE(name, tags) \
  TEMPLATE_TEST_CASE(name, tags, alaio::vm::interpreter)
#endif
