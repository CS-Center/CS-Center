#include "clangpp_executor.hpp"

#if CPP_ENABLED && CLANGPP_ENABLED

  const char* ClangppExecutor::get_ext() { return ".cpp"; }
  
  const char* ClangppExecutor::get_compiler_exec() { return CLANGPP_PATH; }
  
  const char* ClangppExecutor::get_compiler_name() { return "clang++"; }
  
  std::vector<const char*> ClangppExecutor::get_compiler_flags() {
    return {
      "-O", "-Wall", "-DONLINE_JUDGE", get_standard(), "-o", compiled_filename.c_str(),
    };
  }
  
  #if CLANGPP_98_ENABLED
    const char* Clangpp98Executor::get_standard() {
      return "-std=c++98";
    }
  #endif // CLANGPP_98_ENABLED
  
  #if CLANGPP_03_ENABLED
    const char* Clangpp03Executor::get_standard() {
      return "-std=c++03";
    }
  #endif // CLANGPP_03_ENABLED
  
  #if CLANGPP_11_ENABLED
    const char* Clangpp11Executor::get_standard() {
      return "-std=c++11";
    }
  #endif // CLANGPP_11_ENABLED
  
  #if CLANGPP_14_ENABLED
    const char* Clangpp14Executor::get_standard() {
      return "-std=c++14";
    }
  #endif // CLANGPP_14_ENABLED
  
  #if CLANGPP_17_ENABLED
    const char* Clangpp17Executor::get_standard() {
      return "-std=c++17";
    }
  #endif // CLANGPP_17_ENABLED

#endif // CPP_ENABLED && CLANGPP_ENABLED