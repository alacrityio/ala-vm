#include <alaio/vm/backend.hpp>
#include <alaio/vm/host_function.hpp>

#include "utils.hpp"

#include <catch2/catch.hpp>
#include <exception>

using namespace alaio::vm;

static int check = 0;
static int check2 = 0;

void test_func_a(int a, float b) {
}
void test_func_b(char* a, float* b, int& c) {
}
void test_func_c(float a, const char* b, span<const char> c, int d) {
   check = d;
}
void test_func_d() {
}
void test_func_e(float a, const char* b, span<const char> c, int d) {
   check = d;
}

template <typename Ex, typename F>
bool check_exception_msg(F&& func, const std::string& chck) {
   try {
      func();
   } catch( Ex ex ) {
      return ex.what() == chck;
   }
   return false;
}

#define CHECK_MSG(Msg, ...) \
   CHECK(check_exception_msg<std::runtime_error>([&](){ __VA_ARGS__; }, Msg))

EOS_VM_PRECONDITION(int_equals_42,
      EOS_VM_INVOKE_ON(int, [&](auto arg, auto&&... rest) {
         if (arg != 42)
            throw std::runtime_error("arg != 42");
      }));

EOS_VM_PRECONDITION(float_equals_42,
      EOS_VM_INVOKE_ON(float, [&](auto arg, auto&&... rest) {
         if (arg != 42.42f)
            throw std::runtime_error("arg != 42.42f");
      }));

EOS_VM_PRECONDITION(str_equals_hello,
      EOS_VM_INVOKE_ON(const char*, [&](auto arg, auto&&... rest) {
         if (memcmp(arg, "hello", 5) != 0)
            throw std::runtime_error("str != hello");
      }));

EOS_VM_PRECONDITION(span_and_check,
      EOS_VM_INVOKE_ON_ALL([&](auto arg, auto&&... rest) {
         if constexpr (is_span_type_v<decltype(arg)>) {
            if (memcmp(arg.data(), "hellohe", arg.size_bytes()) != 0)
               throw std::runtime_error("span<T> != hellohe");
         }
      }));

EOS_VM_PRECONDITION(check2_once,
      EOS_VM_INVOKE_ONCE([&](auto&&... args) {
         check2++;
      }));

EOS_VM_PRECONDITION(check2_all,
      EOS_VM_INVOKE_ON_ALL([&](auto&&... args) {
         check2++;
      }));

struct cnv : type_converter<standalone_function_t> {
   using type_converter::from_wasm;
   using type_converter::type_converter;
   EOS_VM_FROM_WASM(char*, (void* ptr)) { return static_cast<char*>(ptr); }
   EOS_VM_FROM_WASM(const char*, (void* ptr)) { return static_cast<char*>(ptr); }
   EOS_VM_FROM_WASM(float*, (void* ptr)) { return static_cast<float*>(ptr); }
   EOS_VM_FROM_WASM(int&, (void* ptr)) { return *static_cast<int*>(ptr); }
};

BACKEND_TEST_CASE("Testing invoke_on", "[preconditions_tests]") {
   using rhf_t = registered_host_functions<standalone_function_t, execution_interface, cnv>;
   using backend_t = backend<rhf_t, TestType>;

/*
(module
  (type (;0;) (func (param i32 i32 i32)))
  (type (;1;) (func (param i32 f32)))
  (type (;2;) (func (param)))
  (type (;3;) (func (param f32 i32 i32 i32 i32)))
  (import "env" "test_func_b" (func (;0;) (type 0)))
  (import "env" "test_func_a" (func (;1;) (type 1)))
  (import "env" "test_func_c" (func (;2;) (type 3)))
  (import "env" "test_func_d" (func (;3;) (type 2)))
  (import "env" "test_func_e" (func (;4;) (type 3)))
  (func (;5;) (type 2)
    i32.const 42
    f32.const 42.42
    call 1)
  (func (;6;) (type 2)
    i32.const 8208
    i32.const 8232
    i32.const 8232
    call 0)
  (func (;7;) (type 2)
    i32.const 32
    f32.const 42
    call 1)
  (func (;8;) (type 2)
    i32.const 42
    f32.const 32
    call 1)
  (func (;9;) (type 2)
    i32.const 0
    i32.const 0
    i32.const 0
    call 0)
  (func (;10;) (type 2)
    f32.const 42.42
    i32.const 8208
    i32.const 8208
    i32.const 7
    i32.const 77
    call 2)
  (func (;11;) (type 2)
    f32.const 42.42
    i32.const 8232
    i32.const 8232
    i32.const 4
    i32.const 77
    call 2)
  (func (;12;) (type 2)
    call 3)
  (func (;13;) (type 2)
    f32.const 42.42
    i32.const 8232
    i32.const 8232
    i32.const 4
    i32.const 77
    call 4)
  (memory (;0;) 1)
  (export "call_test_a" (func 5))
  (export "call_test_b" (func 6))
  (export "call_test_c" (func 7))
  (export "call_test_d" (func 8))
  (export "call_test_e" (func 9))
  (export "call_test_f" (func 10))
  (export "call_test_g" (func 11))
  (export "call_test_h" (func 12))
  (export "call_test_j" (func 13))
  (data (i32.const 8208) "hellohello\00")
  (data (i32.const 8232) "\ff\ff\ff\ff"))
*/
   wasm_code test_wasm = {
      0x00, 0x61, 0x73, 0x6d, 0x01, 0x00, 0x00, 0x00, 0x01, 0x17, 0x04, 0x60,
      0x03, 0x7f, 0x7f, 0x7f, 0x00, 0x60, 0x02, 0x7f, 0x7d, 0x00, 0x60, 0x00,
      0x00, 0x60, 0x05, 0x7d, 0x7f, 0x7f, 0x7f, 0x7f, 0x00, 0x02, 0x5b, 0x05,
      0x03, 0x65, 0x6e, 0x76, 0x0b, 0x74, 0x65, 0x73, 0x74, 0x5f, 0x66, 0x75,
      0x6e, 0x63, 0x5f, 0x62, 0x00, 0x00, 0x03, 0x65, 0x6e, 0x76, 0x0b, 0x74,
      0x65, 0x73, 0x74, 0x5f, 0x66, 0x75, 0x6e, 0x63, 0x5f, 0x61, 0x00, 0x01,
      0x03, 0x65, 0x6e, 0x76, 0x0b, 0x74, 0x65, 0x73, 0x74, 0x5f, 0x66, 0x75,
      0x6e, 0x63, 0x5f, 0x63, 0x00, 0x03, 0x03, 0x65, 0x6e, 0x76, 0x0b, 0x74,
      0x65, 0x73, 0x74, 0x5f, 0x66, 0x75, 0x6e, 0x63, 0x5f, 0x64, 0x00, 0x02,
      0x03, 0x65, 0x6e, 0x76, 0x0b, 0x74, 0x65, 0x73, 0x74, 0x5f, 0x66, 0x75,
      0x6e, 0x63, 0x5f, 0x65, 0x00, 0x03, 0x03, 0x0a, 0x09, 0x02, 0x02, 0x02,
      0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x05, 0x03, 0x01, 0x00, 0x01, 0x07,
      0x7f, 0x09, 0x0b, 0x63, 0x61, 0x6c, 0x6c, 0x5f, 0x74, 0x65, 0x73, 0x74,
      0x5f, 0x61, 0x00, 0x05, 0x0b, 0x63, 0x61, 0x6c, 0x6c, 0x5f, 0x74, 0x65,
      0x73, 0x74, 0x5f, 0x62, 0x00, 0x06, 0x0b, 0x63, 0x61, 0x6c, 0x6c, 0x5f,
      0x74, 0x65, 0x73, 0x74, 0x5f, 0x63, 0x00, 0x07, 0x0b, 0x63, 0x61, 0x6c,
      0x6c, 0x5f, 0x74, 0x65, 0x73, 0x74, 0x5f, 0x64, 0x00, 0x08, 0x0b, 0x63,
      0x61, 0x6c, 0x6c, 0x5f, 0x74, 0x65, 0x73, 0x74, 0x5f, 0x65, 0x00, 0x09,
      0x0b, 0x63, 0x61, 0x6c, 0x6c, 0x5f, 0x74, 0x65, 0x73, 0x74, 0x5f, 0x66,
      0x00, 0x0a, 0x0b, 0x63, 0x61, 0x6c, 0x6c, 0x5f, 0x74, 0x65, 0x73, 0x74,
      0x5f, 0x67, 0x00, 0x0b, 0x0b, 0x63, 0x61, 0x6c, 0x6c, 0x5f, 0x74, 0x65,
      0x73, 0x74, 0x5f, 0x68, 0x00, 0x0c, 0x0b, 0x63, 0x61, 0x6c, 0x6c, 0x5f,
      0x74, 0x65, 0x73, 0x74, 0x5f, 0x6a, 0x00, 0x0d, 0x0a, 0x8b, 0x01, 0x09,
      0x0b, 0x00, 0x41, 0x2a, 0x43, 0x14, 0xae, 0x29, 0x42, 0x10, 0x01, 0x0b,
      0x10, 0x00, 0x41, 0x90, 0xc0, 0x00, 0x41, 0xa8, 0xc0, 0x00, 0x41, 0xa8,
      0xc0, 0x00, 0x10, 0x00, 0x0b, 0x0b, 0x00, 0x41, 0x20, 0x43, 0x00, 0x00,
      0x28, 0x42, 0x10, 0x01, 0x0b, 0x0b, 0x00, 0x41, 0x2a, 0x43, 0x00, 0x00,
      0x00, 0x42, 0x10, 0x01, 0x0b, 0x0a, 0x00, 0x41, 0x00, 0x41, 0x00, 0x41,
      0x00, 0x10, 0x00, 0x0b, 0x16, 0x00, 0x43, 0x14, 0xae, 0x29, 0x42, 0x41,
      0x90, 0xc0, 0x00, 0x41, 0x90, 0xc0, 0x00, 0x41, 0x07, 0x41, 0xcd, 0x00,
      0x10, 0x02, 0x0b, 0x16, 0x00, 0x43, 0x14, 0xae, 0x29, 0x42, 0x41, 0xa8,
      0xc0, 0x00, 0x41, 0xa8, 0xc0, 0x00, 0x41, 0x04, 0x41, 0xcd, 0x00, 0x10,
      0x02, 0x0b, 0x04, 0x00, 0x10, 0x03, 0x0b, 0x16, 0x00, 0x43, 0x14, 0xae,
      0x29, 0x42, 0x41, 0xa8, 0xc0, 0x00, 0x41, 0xa8, 0xc0, 0x00, 0x41, 0x04,
      0x41, 0xcd, 0x00, 0x10, 0x04, 0x0b, 0x0b, 0x1e, 0x02, 0x00, 0x41, 0x90,
      0xc0, 0x00, 0x0b, 0x0b, 0x68, 0x65, 0x6c, 0x6c, 0x6f, 0x68, 0x65, 0x6c,
      0x6c, 0x6f, 0x00, 0x00, 0x41, 0xa8, 0xc0, 0x00, 0x0b, 0x04, 0xff, 0xff,
      0xff, 0xff
   };

   rhf_t::add<&test_func_a, int_equals_42, float_equals_42>("env", "test_func_a");
   rhf_t::add<&test_func_b, str_equals_hello>("env", "test_func_b");
   rhf_t::add<&test_func_c, float_equals_42, str_equals_hello, span_and_check>("env", "test_func_c");
   rhf_t::add<&test_func_d, check2_once>("env", "test_func_d");
   rhf_t::add<&test_func_e, check2_all>("env", "test_func_e");

   wasm_allocator wa;
   backend_t bkend(test_wasm, &wa);

   check  = 0;
   check2 = 0;
   bkend("env", "call_test_a");
   bkend("env", "call_test_b");
   CHECK_MSG("arg != 42", bkend("env", "call_test_c"));
   CHECK_MSG("arg != 42.42f", bkend("env", "call_test_d"));
   bkend("env", "call_test_e");
   CHECK(check == 0);
   bkend("env", "call_test_f");
   CHECK(check == 77);

   // reset check
   check = 0;
   CHECK_MSG("str != hello", bkend("env", "call_test_g"));
   CHECK(check == 0);

   bkend("env", "call_test_h");
   CHECK(check2 == 1);
   bkend("env", "call_test_h");
   CHECK(check2 == 2);

   check2 = 0;
   bkend("env", "call_test_j");
   CHECK(check2 == 4);
   bkend("env", "call_test_j");
   CHECK(check2 == 8);
}
