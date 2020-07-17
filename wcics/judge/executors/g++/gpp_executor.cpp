#include "gpp_executor.hpp"

#if defined CPP_ENABLED && defined GPP_ENABLED

  const char* GppExecutor::get_source_ext() { return ".cpp"; }
  
  const char* GppExecutor::get_compiler_exec() { return GPP_PATH; }
  
  const char* GppExecutor::get_compiler_name() { return "g++"; }
  
  std::vector<const char*> GppExecutor::get_compiler_flags() {
    return {
      "-O", "-Wall", "-DONLINE_JUDGE", "-o", compiled_filename.c_str(), get_standard()
    };
  }
  
  #ifdef GPP_98_ENABLED
    const char* Gpp98Executor::get_standard() {
      return "-std=c++98";
    }
  #endif // GPP_98_ENABLED
  
  #ifdef GPP_03_ENABLED
    const char* Gpp03Executor::get_standard() {
      return "-std=c++03";
    }
  #endif // GPP_03_ENABLED
  
  #ifdef GPP_11_ENABLED
    const char* Gpp11Executor::get_standard() {
      return "-std=c++11";
    }
  #endif // GPP_11_ENABLED
  
  #ifdef GPP_14_ENABLED
    const char* Gpp14Executor::get_standard() {
      return "-std=c++14";
    }
  #endif // GPP_14_ENABLED
  
  #ifdef GPP_17_ENABLED
    const char* Gpp17Executor::get_standard() {
      return "-std=c++17";
    }
  #endif // GPP_17_ENABLED

#endif // CPP_ENABLED && GPP_ENABLED