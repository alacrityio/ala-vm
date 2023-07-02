#include <iostream>
#include <list>

#include <alaio/vm/backend.hpp>
#include <alaio/vm/watchdog.hpp>

#include "utils.hpp"
#include <catch2/catch.hpp>

using namespace alaio;
using namespace alaio::vm;

#pragma GCC push_options
#pragma GCC optimize ("O0")
BACKEND_TEST_CASE("Test instantiating multiple instances of ala-vm",
                  "[instantiation_test]") {
   std::vector<uint8_t> test_wasm = {
     0x00, 0x61, 0x73, 0x6d, 0x01, 0x00, 0x00, 0x00, 0x01, 0x04, 0x01, 0x60,
     0x00, 0x00, 0x03, 0x03, 0x02, 0x00, 0x00, 0x04, 0x05, 0x01, 0x70, 0x01,
     0x01, 0x01, 0x05, 0x03, 0x01, 0x00, 0x01, 0x06, 0x15, 0x03, 0x7f, 0x01,
     0x41, 0x90, 0xc8, 0x00, 0x0b, 0x7f, 0x00, 0x41, 0x90, 0xc8, 0x00, 0x0b,
     0x7f, 0x00, 0x41, 0x84, 0x08, 0x0b, 0x07, 0x30, 0x04, 0x06, 0x6d, 0x65,
     0x6d, 0x6f, 0x72, 0x79, 0x02, 0x00, 0x0b, 0x5f, 0x5f, 0x68, 0x65, 0x61,
     0x70, 0x5f, 0x62, 0x61, 0x73, 0x65, 0x03, 0x01, 0x0a, 0x5f, 0x5f, 0x64,
     0x61, 0x74, 0x61, 0x5f, 0x65, 0x6e, 0x64, 0x03, 0x02, 0x08, 0x64, 0x6f,
     0x74, 0x68, 0x65, 0x64, 0x65, 0x77, 0x00, 0x01, 0x0a, 0x8c, 0x02, 0x02,
     0x02, 0x00, 0x0b, 0x86, 0x02, 0x06, 0x01, 0x7f, 0x02, 0x7e, 0x01, 0x7f,
     0x01, 0x7c, 0x02, 0x7f, 0x03, 0x7e, 0x41, 0x00, 0x28, 0x02, 0x80, 0x88,
     0x80, 0x80, 0x00, 0x21, 0x00, 0x42, 0x00, 0x21, 0x01, 0x42, 0x02, 0x21,
     0x02, 0x02, 0x40, 0x03, 0x40, 0x02, 0x40, 0x20, 0x02, 0x42, 0x02, 0x51,
     0x0d, 0x00, 0x02, 0x40, 0x20, 0x02, 0xa7, 0x22, 0x03, 0x41, 0x01, 0x71,
     0x45, 0x0d, 0x00, 0x02, 0x40, 0x02, 0x40, 0x02, 0x40, 0x20, 0x03, 0xb7,
     0x9f, 0x22, 0x04, 0x99, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0,
     0x41, 0x63, 0x0d, 0x00, 0x41, 0x80, 0x80, 0x80, 0x80, 0x78, 0x21, 0x05,
     0x41, 0x80, 0x80, 0x80, 0x80, 0x78, 0x41, 0x03, 0x4e, 0x0d, 0x01, 0x0c,
     0x02, 0x0b, 0x20, 0x04, 0xaa, 0x22, 0x05, 0x41, 0x03, 0x48, 0x0d, 0x01,
     0x0b, 0x41, 0x03, 0x21, 0x06, 0x03, 0x40, 0x20, 0x03, 0x20, 0x06, 0x70,
     0x45, 0x0d, 0x01, 0x20, 0x06, 0x41, 0x02, 0x6a, 0x22, 0x06, 0x20, 0x05,
     0x4c, 0x0d, 0x00, 0x0b, 0x0b, 0x20, 0x02, 0x42, 0x03, 0x54, 0x0d, 0x00,
     0x42, 0x7f, 0x20, 0x02, 0x86, 0x42, 0x7f, 0x85, 0x21, 0x07, 0x42, 0x04,
     0x21, 0x08, 0x20, 0x01, 0x21, 0x09, 0x03, 0x40, 0x20, 0x08, 0x20, 0x08,
     0x7e, 0x42, 0x7e, 0x7c, 0x20, 0x07, 0x82, 0x21, 0x08, 0x20, 0x09, 0x42,
     0x7f, 0x7c, 0x22, 0x09, 0x42, 0x00, 0x52, 0x0d, 0x00, 0x0b, 0x20, 0x08,
     0x42, 0x00, 0x51, 0x0d, 0x01, 0x0b, 0x20, 0x01, 0x42, 0x01, 0x7c, 0x21,
     0x01, 0x20, 0x02, 0x42, 0x01, 0x7c, 0x22, 0x02, 0x42, 0x80, 0x2a, 0x52,
     0x0d, 0x01, 0x0c, 0x02, 0x0b, 0x20, 0x00, 0x41, 0xef, 0xfd, 0xb7, 0xf5,
     0x7d, 0x36, 0x02, 0x00, 0x20, 0x01, 0x42, 0x01, 0x7c, 0x21, 0x01, 0x20,
     0x02, 0x42, 0x01, 0x7c, 0x22, 0x02, 0x42, 0x80, 0x2a, 0x52, 0x0d, 0x00,
     0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x01, 0x00, 0x41, 0x80, 0x08, 0x0b, 0x04,
     0xfc, 0xff, 0x00, 0x00, 0x00, 0x25, 0x04, 0x6e, 0x61, 0x6d, 0x65, 0x01,
     0x1e, 0x02, 0x00, 0x11, 0x5f, 0x5f, 0x77, 0x61, 0x73, 0x6d, 0x5f, 0x63,
     0x61, 0x6c, 0x6c, 0x5f, 0x63, 0x74, 0x6f, 0x72, 0x73, 0x01, 0x08, 0x64,
     0x6f, 0x74, 0x68, 0x65, 0x64, 0x65, 0x77, 0x00, 0x25, 0x09, 0x70, 0x72,
     0x6f, 0x64, 0x75, 0x63, 0x65, 0x72, 0x73, 0x01, 0x0c, 0x70, 0x72, 0x6f,
     0x63, 0x65, 0x73, 0x73, 0x65, 0x64, 0x2d, 0x62, 0x79, 0x01, 0x05, 0x63,
     0x6c, 0x61, 0x6e, 0x67, 0x05, 0x39, 0x2e, 0x30, 0x2e, 0x30
   };

   using backend_t = backend<registered_host_functions<standalone_function_t>, TestType>;
   std::list<backend_t> v;
   for (int i=0; i < 8193; i++)
      v.emplace_back(test_wasm, get_wasm_allocator());
}
#pragma GCC pop_options
