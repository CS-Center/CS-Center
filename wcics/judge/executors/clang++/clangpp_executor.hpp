#pragma once

#include "executors/config/cpp.hpp"

#if HAS_CLANGPP

  #include "executors/compiled_executor.hpp"

  class ClangppExecutor : public CompiledExecutor {
    using CompiledExecutor::CompiledExecutor;
  
    virtual const char* get_standard() = 0;
  
    std::vector<const char*> get_compiler_flags();
  
    const char* get_compiler_exec();
    const char* get_compiler_name();
    
    const char* get_ext();
  };
  
  #if HAS_CLANGPP_98
  
    class Clangpp98Executor : public ClangppExecutor {
      using ClangppExecutor::ClangppExecutor;
    
      const char* get_standard();
    };
  
  #endif // HAS_CLANGPP_98
  
  #if HAS_CLANGPP_03
  
    class Clangpp03Executor : public ClangppExecutor {
      using ClangppExecutor::ClangppExecutor;
    
      const char* get_standard();
    };
  
  #endif // HAS_CLANGPP_03
  
  #if HAS_CLANGPP_11
  
    class Clangpp11Executor : public ClangppExecutor {
      using ClangppExecutor::ClangppExecutor;
    
      const char* get_standard();
    };
  
  #endif // HAS_CLANGPP_11
  
  #if HAS_CLANGPP_14
  
    class Clangpp14Executor : public ClangppExecutor {
      using ClangppExecutor::ClangppExecutor;
    
      const char* get_standard();
    };
  
  #endif // HAS_CLANGPP_14
  
  #if HAS_CLANGPP_17
  
    class Clangpp17Executor : public ClangppExecutor {
      using ClangppExecutor::ClangppExecutor;
    
      const char* get_standard();
    };
  
  #endif // HAS_CLANGPP_17

#endif // HAS_CLANGPP