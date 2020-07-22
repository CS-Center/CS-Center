#pragma once

#include "executors/config/c.hpp"

#if HAS_GCC

  #include "executors/compiled_executor.hpp"

  class GCCExecutor : public CompiledExecutor {
    using CompiledExecutor::CompiledExecutor;
  
    virtual const char* get_standard() = 0;
    
    std::vector<const char*> get_compiler_flags();
  
    const char* get_compiler_exec();
    const char* get_compiler_name();
    
    const char* get_source_ext();
  };
  
  #if HAS_GCC_90
  
    class GCC90Executor : public GCCExecutor {
      using GCCExecutor::GCCExecutor;
    
      const char* get_standard();
    };
  
  #endif // HAS_GCC_90
  
  #if HAS_GCC_99
  
    class GCC99Executor : public GCCExecutor {
      using GCCExecutor::GCCExecutor;
      
      const char* get_standard();
    };
  
  #endif // HAS_GCC_99
  
  #if HAS_GCC_11
  
    class GCC11Executor : public GCCExecutor {
      using GCCExecutor::GCCExecutor;
      
      const char* get_standard();
    };
  
  #endif // HAS_GCC_11

#endif // HAS_GCC