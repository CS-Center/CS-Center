#include "gcc_executor.hpp"

#if defined C_ENABLED && defined GCC_ENABLED

  const char* GCCExecutor::get_source_ext() { return ".c"; }
  
  const char* GCCExecutor::get_compiler_exec() { return GCC_PATH; }
  
  const char* GCCExecutor::get_compiler_name() { return "gcc"; }
  
  std::vector<const char*> GCCExecutor::get_compiler_flags() {
    return {
      "-Wall", "-DONLINE_JUDGE", "-O", get_standard(), "-o", compiled_filename.c_str()
    };
  }
  
  #ifdef GCC_90_ENABLED
    const char* GCC90Executor::get_standard() {
      return "-std=c90";
    }
  #endif // GCC_90_ENABLED
  
  #ifdef GCC_99_ENABLED
    const char* GCC99Executor::get_standard() {
      return "-std=c99";
    }
  #endif // GCC_99_ENABLED
  
  #ifdef GCC_11_ENABLED
    const char* GCC11Executor::get_standard() {
      return "-std=c11";
    }
  #endif // GCC_11_ENABLED

#endif // C_ENABLED && GCC_ENABLED