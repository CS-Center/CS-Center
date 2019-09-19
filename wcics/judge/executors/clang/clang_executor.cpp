#include "clang_executor.hpp"

#if defined C_ENABLED && defined CLANG_ENABLED

  const char* ClangExecutor::get_ext() { return "c"; }
  
  const char* ClangExecutor::get_compiler() { return CLANG_PATH; }
  
  const char* ClangExecutor::get_compiler_name() { return "clang"; }
  
  int ClangExecutor::make_compiler_args() {
    const char* base_cargs[] = {
      "clang", "-O", "-Wall", "-DONLINE_JUDGE", get_standard(), "-o", file, filepath, 0
    };
    
    return copy_compiler_args(base_cargs);
  }
  
  #ifdef CLANG_90_ENABLED
    const char* Clang90Executor::get_standard() {
      return "-std=c90";
    }
  #endif // CLANG_90_ENABLED
  
  #ifdef CLANG_99_ENABLED
    const char* Clang99Executor::get_standard() {
      return "-std=c99";
    }
  #endif // CLANG_99_ENABLED
  
  #ifdef CLANG_11_ENABLED
    const char* Clang11Executor::get_standard() {
      return "-std=c11";
    }
  #endif // CLANG_11_ENABLED

#endif // C_ENABLED && GCC_ENABLED