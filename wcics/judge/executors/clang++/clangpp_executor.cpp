#include "clangpp_executor.hpp"

#if HAS_CLANGPP

  const char* ClangppExecutor::get_ext() { return ".cpp"; }
  
  const char* ClangppExecutor::get_compiler_exec() { return CLANGPP_PATH; }
  
  const char* ClangppExecutor::get_compiler_name() { return "clang++"; }
  
  std::vector<const char*> ClangppExecutor::get_compiler_flags() {
    return {
      "-O", "-Wall", "-DONLINE_JUDGE", get_standard(), "-o", compiled_filename.c_str(),
    };
  }
  
  #if HAS_CLANGPP_98
    const char* Clangpp98Executor::get_standard() {
      return "-std=c++98";
    }
  #endif // HAS_CLANGPP_98
  
  #if HAS_CLANGPP_03
    const char* Clangpp03Executor::get_standard() {
      return "-std=c++03";
    }
  #endif // HAS_CLANGPP_03
  
  #if HAS_CLANGPP_11
    const char* Clangpp11Executor::get_standard() {
      return "-std=c++11";
    }
  #endif // HAS_CLANGPP_11
  
  #if HAS_CLANGPP_14
    const char* Clangpp14Executor::get_standard() {
      return "-std=c++14";
    }
  #endif // HAS_CLANGPP_14
  
  #if HAS_CLANGPP_17
    const char* Clangpp17Executor::get_standard() {
      return "-std=c++17";
    }
  #endif // HAS_CLANGPP_17

#endif // HAS_CLANGPP