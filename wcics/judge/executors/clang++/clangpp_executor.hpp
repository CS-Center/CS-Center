#pragma once

#include "runtimes/cpp.hpp"

#if CPP_ENABLED && CLANGPP_ENABLED

  #include "executors/compiled_executor.hpp"

  class ClangppExecutor : public CompiledExecutor {
    using CompiledExecutor::CompiledExecutor;
  
    virtual const char* get_standard() = 0;
  
    std::vector<const char*> get_compiler_flags();
  
    const char* get_compiler_exec();
    const char* get_compiler_name();
    
    const char* get_ext();
  };
  
  #if CLANGPP_98_ENABLED
  
    class Clangpp98Executor : public ClangppExecutor {
      using ClangppExecutor::ClangppExecutor;
    
      const char* get_standard();
    };
  
  #endif // CLANGPP_98_ENABLED
  
  #if CLANGPP_03_ENABLED
  
    class Clangpp03Executor : public ClangppExecutor {
      using ClangppExecutor::ClangppExecutor;
    
      const char* get_standard();
    };
  
  #endif // CLANGPP_03_ENABLED
  
  #if CLANGPP_11_ENABLED
  
    class Clangpp11Executor : public ClangppExecutor {
      using ClangppExecutor::ClangppExecutor;
    
      const char* get_standard();
    };
  
  #endif // CLANGPP_11_ENABLED
  
  #if CLANGPP_14_ENABLED
  
    class Clangpp14Executor : public ClangppExecutor {
      using ClangppExecutor::ClangppExecutor;
    
      const char* get_standard();
    };
  
  #endif // CLANGPP_14_ENABLED
  
  #if CLANGPP_17_ENABLED
  
    class Clangpp17Executor : public ClangppExecutor {
      using ClangppExecutor::ClangppExecutor;
    
      const char* get_standard();
    };
  
  #endif // CLANGPP_17_ENABLED

#endif // CPP_ENABLED && CLANGPP_ENABLED