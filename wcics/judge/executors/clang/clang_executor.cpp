#include "clang_executor.hpp"

#if HAS_CLANG

  const char* ClangExecutor::get_source_ext() { return ".c"; }
  
  const char* ClangExecutor::get_compiler_exec() { return CLANG_PATH; }
  
  const char* ClangExecutor::get_compiler_name() { return "clang"; }
  
  std::vector<const char*> ClangExecutor::get_compiler_flags() {
    return {
      "-O", "-Wall", "-DONLINE_JUDGE", get_standard(), "-o", compiled_filename.c_str()
    };
  }
  
  #if HAS_CLANG_90
    const char* Clang90Executor::get_standard() {
      return "-std=c90";
    }
  #endif // HAS_CLANG_90
  
  #if HAS_CLANG_99
    const char* Clang99Executor::get_standard() {
      return "-std=c99";
    }
  #endif // HAS_CLANG_99
  
  #if HAS_CLANG_11
    const char* Clang11Executor::get_standard() {
      return "-std=c11";
    }
  #endif // HAS_CLANG_11

#endif // HAS_CLANG