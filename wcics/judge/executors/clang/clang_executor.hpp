#pragma once

#include "runtimes/c.hpp"

#if defined C_ENABLED && defined CLANG_ENABLED

  #include "executors/compiled_executor.hpp"

  class ClangExecutor : public CompiledExecutor {
    using CompiledExecutor::CompiledExecutor;
  
    virtual const char* get_standard() = 0;
  
    int make_compiler_args();
  
    const char* get_compiler();
    const char* get_compiler_name();
    
    const char* get_ext();
  };
  
  #ifdef CLANG_90_ENABLED
  
    class Clang90Executor : public ClangExecutor {
      using ClangExecutor::ClangExecutor;
    
      const char* get_standard();
    };
  
  #endif // CLANG_90_ENABLED
  
  #ifdef CLANG_99_ENABLED
  
    class Clang99Executor : public ClangExecutor {
      using ClangExecutor::ClangExecutor;

      const char* get_standard();
    };
  
  #endif // CLANG_99_ENABLED
  
  #ifdef CLANG_11_ENABLED
  
    class Clang11Executor : public ClangExecutor {
      using ClangExecutor::ClangExecutor;
    
      const char* get_standard();
    };
  
  #endif // CLANG_11_ENABLED

#endif // C_ENABLED && GCC_ENABLED