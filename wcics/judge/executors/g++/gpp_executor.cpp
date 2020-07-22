#include "gpp_executor.hpp"

#if HAS_GPP

  const char* GppExecutor::get_source_ext() { return ".cpp"; }
  
  const char* GppExecutor::get_compiler_exec() { return GPP_PATH; }
  
  const char* GppExecutor::get_compiler_name() { return "g++"; }
  
  std::vector<const char*> GppExecutor::get_compiler_flags() {
    return {
      "-O", "-Wall", "-DONLINE_JUDGE", "-o", compiled_filename.c_str(), get_standard()
    };
  }
  
  #ifdef HAS_GPP_98
    const char* Gpp98Executor::get_standard() {
      return "-std=c++98";
    }
  #endif // HAS_GPP_98
  
  #ifdef HAS_GPP_03
    const char* Gpp03Executor::get_standard() {
      return "-std=c++03";
    }
  #endif // HAS_GPP_03
  
  #ifdef HAS_GPP_11
    const char* Gpp11Executor::get_standard() {
      return "-std=c++11";
    }
  #endif // HAS_GPP_11
  
  #ifdef HAS_GPP_14
    const char* Gpp14Executor::get_standard() {
      return "-std=c++14";
    }
  #endif // HAS_GPP_14
  
  #ifdef HAS_GPP_17
    const char* Gpp17Executor::get_standard() {
      return "-std=c++17";
    }
  #endif // HAS_GPP_17

#endif // HAS_GPP