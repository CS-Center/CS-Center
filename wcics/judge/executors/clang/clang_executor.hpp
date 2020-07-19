#pragma once

#include "runtimes/c.hpp"

#if C_ENABLED && CLANG_ENABLED

  #include "executors/compiled_executor.hpp"

  class ClangExecutor : public CompiledExecutor {
    using CompiledExecutor::CompiledExecutor;
  
    virtual const char* get_standard() = 0;
  
    std::vector<const char*> get_compiler_flags();
  
    const char* get_compiler_exec();
    const char* get_compiler_name();
    
    const char* get_source_ext();
  };
  
  #if CLANG_90_ENABLED
  
    class Clang90Executor : public ClangExecutor {
      using ClangExecutor::ClangExecutor;
    
      const char* get_standard();
    };
  
  #endif // CLANG_90_ENABLED
  
  #if CLANG_99_ENABLED
  
    class Clang99Executor : public ClangExecutor {
      using ClangExecutor::ClangExecutor;

      const char* get_standard();
    };
  
  #endif // CLANG_99_ENABLED
  
  #if CLANG_11_ENABLED
  
    class Clang11Executor : public ClangExecutor {
      using ClangExecutor::ClangExecutor;
    
      const char* get_standard();
    };
  
  #endif // CLANG_11_ENABLED

#endif // C_ENABLED && GCC_ENABLED