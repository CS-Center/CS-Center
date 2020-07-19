#pragma once

#include "runtimes/cpp.hpp"

#if CPP_ENABLED && GPP_ENABLED

  #include "executors/compiled_executor.hpp"

  class GppExecutor : public CompiledExecutor {
    using CompiledExecutor::CompiledExecutor;
  
    virtual const char* get_standard() = 0;
  
    std::vector<const char*> get_compiler_flags();
  
    const char* get_compiler_exec();
    const char* get_compiler_name();
    
    const char* get_source_ext();
  };
  
  #if GPP_98_ENABLED
  
    class Gpp98Executor : public GppExecutor {
      using GppExecutor::GppExecutor;
    
      const char* get_standard();
    };
  
  #endif // GPP_98_ENABLED
  
  #if GPP_03_ENABLED
    
    class Gpp03Executor : public GppExecutor {
      using GppExecutor::GppExecutor;
    
      const char* get_standard();
    };
    
  #endif // GPP_03_ENABLED
  
  #if GPP_11_ENABLED
  
    class Gpp11Executor : public GppExecutor {
      using GppExecutor::GppExecutor;
    
      const char* get_standard();
    };
    
  #endif // GPP_11_ENABLED
  
  #if GPP_14_ENABLED
  
    class Gpp14Executor : public GppExecutor {
      using GppExecutor::GppExecutor;
    
      const char* get_standard();
    };
    
  #endif // GPP_14_ENABLED
  
  #if GPP_17_ENABLED
  
    class Gpp17Executor : public GppExecutor {
      using GppExecutor::GppExecutor;
    
      const char* get_standard();
    };
    
  #endif // GPP_17_ENABLED

#endif // CPP_ENABLED && GPP_ENABLED