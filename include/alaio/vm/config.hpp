#pragma once

namespace alaio { namespace vm {

// create constexpr flags for whether the backend should obey alignment hints
#ifdef ALA_VM_ALIGN_MEMORY_OPS
   inline constexpr bool should_align_memory_ops = true;
#else
   inline constexpr bool should_align_memory_ops = false;
#endif


#ifdef ALA_VM_SOFTFLOAT
   inline constexpr bool use_softfloat = true;
#else
   inline constexpr bool use_softfloat = false;
#endif

#ifdef ALA_VM_FULL_DEBUG
   inline constexpr bool ala_vm_debug = true;
#else
   inline constexpr bool ala_vm_debug = false;
#endif

#ifdef __x86_64__
   inline constexpr bool ala_vm_amd64 = true;
#else
   inline constexpr bool ala_vm_amd64 = false;
#endif

}} // namespace alaio::vm
