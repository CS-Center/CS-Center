#include "clang_executor.hpp"

#if defined C_ENABLED && defined CLANG_ENABLED

  const char* ClangExecutor::get_source_ext() { return ".c"; }
  
  const char* ClangExecutor::get_compiler_exec() { return CLANG_PATH; }
  
  const char* ClangExecutor::get_compiler_name() { return "clang"; }
  
  std::vector<const char*> ClangExecutor::get_compiler_flags() {
    return {
      "-O", "-Wall", "-DONLINE_JUDGE", get_standard(), "-o", compiled_filename.c_str()
    };
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