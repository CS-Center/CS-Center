#pragma once

#include "executors/config/c.hpp"

#if HAS_CLANG

  #include "executors/compiled_executor.hpp"

  class ClangExecutor : public CompiledExecutor {
    using CompiledExecutor::CompiledExecutor;
  
    virtual const char* get_standard() = 0;
  
    std::vector<const char*> get_compiler_flags();
  
    const char* get_compiler_exec();
    const char* get_compiler_name();
    
    const char* get_source_ext();
  };
  
  #if HAS_CLANG_90
  
    class Clang90Executor : public ClangExecutor {
      using ClangExecutor::ClangExecutor;
    
      const char* get_standard();
    };
  
  #endif // HAS_CLANG_90
  
  #if HAS_CLANG_99
  
    class Clang99Executor : public ClangExecutor {
      using ClangExecutor::ClangExecutor;

      const char* get_standard();
    };
  
  #endif // HAS_CLANG_99
  
  #if HAS_CLANG_11
  
    class Clang11Executor : public ClangExecutor {
      using ClangExecutor::ClangExecutor;
    
      const char* get_standard();
    };
  
  #endif // HAS_CLANG_11

#endif // HAS_CLANG