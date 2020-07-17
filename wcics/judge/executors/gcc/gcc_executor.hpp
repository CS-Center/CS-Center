#pragma once

#include "runtimes/c.hpp"

#if defined C_ENABLED && defined GCC_ENABLED

  #include "executors/compiled_executor.hpp"

  class GCCExecutor : public CompiledExecutor {
    using CompiledExecutor::CompiledExecutor;
  
    virtual const char* get_standard() = 0;
    
    std::vector<const char*> get_compiler_flags();
  
    const char* get_compiler_exec();
    const char* get_compiler_name();
    
    const char* get_source_ext();
  };
  
  #ifdef GCC_90_ENABLED
  
    class GCC90Executor : public GCCExecutor {
      using GCCExecutor::GCCExecutor;
    
      const char* get_standard();
    };
  
  #endif // GCC_90_ENABLED
  
  #ifdef GCC_99_ENABLED
  
    class GCC99Executor : public GCCExecutor {
      using GCCExecutor::GCCExecutor;
      
      const char* get_standard();
    };
  
  #endif // GCC_99_ENABLED
  
  #ifdef GCC_11_ENABLED
  
    class GCC11Executor : public GCCExecutor {
      using GCCExecutor::GCCExecutor;
      
      const char* get_standard();
    };
  
  #endif // GCC_11_ENABLED

#endif // C_ENABLED && GCC_ENABLED