#pragma once

#include "runtimes/cpp.hpp"

#if defined CPP_ENABLED && defined CLANGPP_ENABLED

  #include "executors/compiled_executor.hpp"

  class ClangppExecutor : public CompiledExecutor {
    using CompiledExecutor::CompiledExecutor;
  
    virtual const char* get_standard() = 0;
  
    int make_compiler_args();
  
    const char* get_compiler();
    const char* get_compiler_name();
    
    const char* get_ext();
  };
  
  #ifdef CLANGPP_98_ENABLED
  
    class Clangpp98Executor : public ClangppExecutor {
      using ClangppExecutor::ClangppExecutor;
    
      const char* get_standard();
    };
  
  #endif // CLANGPP_98_ENABLED
  
  #ifdef CLANGPP_03_ENABLED
  
    class Clangpp03Executor : public ClangppExecutor {
      using ClangppExecutor::ClangppExecutor;
    
      const char* get_standard();
    };
  
  #endif // CLANGPP_03_ENABLED
  
  #ifdef CLANGPP_11_ENABLED
  
    class Clangpp11Executor : public ClangppExecutor {
      using ClangppExecutor::ClangppExecutor;
    
      const char* get_standard();
    };
  
  #endif // CLANGPP_11_ENABLED
  
  #ifdef CLANGPP_14_ENABLED
  
    class Clangpp14Executor : public ClangppExecutor {
      using ClangppExecutor::ClangppExecutor;
    
      const char* get_standard();
    };
  
  #endif // CLANGPP_14_ENABLED
  
  #ifdef CLANGPP_17_ENABLED
  
    class Clangpp17Executor : public ClangppExecutor {
      using ClangppExecutor::ClangppExecutor;
    
      const char* get_standard();
    };
  
  #endif // CLANGPP_17_ENABLED

#endif // CPP_ENABLED && CLANGPP_ENABLED