#include "gcc_executor.hpp"

#if HAS_GCC

  const char* GCCExecutor::get_source_ext() { return ".c"; }
  
  const char* GCCExecutor::get_compiler_exec() { return GCC_PATH; }
  
  const char* GCCExecutor::get_compiler_name() { return "gcc"; }
  
  std::vector<const char*> GCCExecutor::get_compiler_flags() {
    return {
      "-Wall", "-DONLINE_JUDGE", "-O", get_standard(), "-o", compiled_filename.c_str()
    };
  }
  
  #if HAS_GCC_90
    const char* GCC90Executor::get_standard() {
      return "-std=c90";
    }
  #endif // HAS_GCC_90
  
  #if HAS_GCC_99
    const char* GCC99Executor::get_standard() {
      return "-std=c99";
    }
  #endif // HAS_GCC_99
  
  #if HAS_GCC_11
    const char* GCC11Executor::get_standard() {
      return "-std=c11";
    }
  #endif // HAS_GCC_11

#endif // HAS_GCC