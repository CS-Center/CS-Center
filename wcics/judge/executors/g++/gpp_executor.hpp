#pragma once

#include "runtimes/cpp.hpp"

#if defined CPP_ENABLED && defined GPP_ENABLED

  #include "executors/compiled_executor.hpp"

  class GppExecutor : public CompiledExecutor {
    using CompiledExecutor::CompiledExecutor;
  
    virtual const char* get_standard() = 0;
  
    int make_compiler_args();
  
    const char* get_compiler();
    const char* get_compiler_name();
    
    const char* get_ext();
  };
  
  #ifdef GPP_98_ENABLED
  
    class Gpp98Executor : public GppExecutor {
      using GppExecutor::GppExecutor;
    
      const char* get_standard();
    };
  
  #endif // GPP_98_ENABLED
  
  #ifdef GPP_03_ENABLED
    
    class Gpp03Executor : public GppExecutor {
      using GppExecutor::GppExecutor;
    
      const char* get_standard();
    };
    
  #endif // GPP_03_ENABLED
  
  #ifdef GPP_11_ENABLED
  
    class Gpp11Executor : public GppExecutor {
      using GppExecutor::GppExecutor;
    
      const char* get_standard();
    };
    
  #endif // GPP_11_ENABLED
  
  #ifdef GPP_14_ENABLED
  
    class Gpp14Executor : public GppExecutor {
      using GppExecutor::GppExecutor;
    
      const char* get_standard();
    };
    
  #endif // GPP_14_ENABLED
  
  #ifdef GPP_17_ENABLED
  
    class Gpp17Executor : public GppExecutor {
      using GppExecutor::GppExecutor;
    
      const char* get_standard();
    };
    
  #endif // GPP_17_ENABLED

#endif // CPP_ENABLED && GPP_ENABLED