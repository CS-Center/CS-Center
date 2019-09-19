#include "clangpp_executor.hpp"

#if defined CPP_ENABLED && defined CLANGPP_ENABLED

  const char* ClangppExecutor::get_ext() { return "cpp"; }
  
  const char* ClangppExecutor::get_compiler() { return CLANGPP_PATH; }
  
  const char* ClangppExecutor::get_compiler_name() { return "clang++"; }
  
  int ClangppExecutor::make_compiler_args() {
    const char* base_cargs[] = {
      "clang++", "-O", "-Wall", "-DONLINE_JUDGE", get_standard(), "-o", file, filepath, 0
    };
    
    return copy_compiler_args(base_cargs);
  }
  
  #ifdef CLANGPP_98_ENABLED
    const char* Clangpp98Executor::get_standard() {
      return "-std=c++98";
    }
  #endif // CLANGPP_98_ENABLED
  
  #ifdef CLANGPP_03_ENABLED
    const char* Clangpp03Executor::get_standard() {
      return "-std=c++03";
    }
  #endif // CLANGPP_03_ENABLED
  
  #ifdef CLANGPP_11_ENABLED
    const char* Clangpp11Executor::get_standard() {
      return "-std=c++11";
    }
  #endif // CLANGPP_11_ENABLED
  
  #ifdef CLANGPP_14_ENABLED
    const char* Clangpp14Executor::get_standard() {
      return "-std=c++14";
    }
  #endif // CLANGPP_14_ENABLED
  
  #ifdef CLANGPP_17_ENABLED
    const char* Clangpp17Executor::get_standard() {
      return "-std=c++17";
    }
  #endif // CLANGPP_17_ENABLED

#endif // CPP_ENABLED && CLANGPP_ENABLED