#include <alaio/vm/backend.hpp>
#include <alaio/vm/error_codes.hpp>
#include <alaio/vm/watchdog.hpp>

#include <iostream>

using namespace alaio;
using namespace alaio::vm;

/**
 * Simple implementation of an interpreter using ala-vm.
 */
int main(int argc, char** argv) {
   // Thread specific `allocator` used for wasm linear memory.
   wasm_allocator wa;

   if (argc < 2) {
      std::cerr << "Error, no wasm file provided\n";
      return -1;
   }

   std::string filename = argv[1];

   watchdog wd{std::chrono::seconds(3)};

   try {
      // Read the wasm into memory.
      auto code = read_wasm( filename );

      // Instaniate a new backend using the wasm provided.
      backend<std::nullptr_t, interpreter, default_options> bkend( code, &wa );

      // Execute any exported functions provided by the wasm.
      bkend.execute_all(wd);

   } catch ( const alaio::vm::exception& ex ) {
      std::cerr << "ala-vm interpreter error\n";
      std::cerr << ex.what() << " : " << ex.detail() << "\n";
   }
   return 0;
}
