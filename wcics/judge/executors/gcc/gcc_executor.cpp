#include "gcc_executor.hpp"

#if defined C_ENABLED && defined GCC_ENABLED

  const char* GCCExecutor::get_ext() { return "c"; }
  
  const char* GCCExecutor::get_compiler() { return GCC_PATH; }
  
  const char* GCCExecutor::get_compiler_name() { return "gcc"; }
  
  int GCCExecutor::make_compiler_args() {
    const char* base_cargs[] = {
      "gcc", "-Wall", "-DONLINE_JUDGE", "-O", "-o", get_standard(), file, filepath, 0
    };
    
    return copy_compiler_args(base_cargs);
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