#pragma once

#include "executors/config/cpp.hpp"

#if HAS_GPP

  #include "executors/compiled_executor.hpp"

  class GppExecutor : public CompiledExecutor {
    using CompiledExecutor::CompiledExecutor;
  
    virtual const char* get_standard() = 0;
  
    std::vector<const char*> get_compiler_flags();
  
    const char* get_compiler_exec();
    const char* get_compiler_name();
    
    const char* get_source_ext();
  };
  
  #if HAS_GPP_98
  
    class Gpp98Executor : public GppExecutor {
      using GppExecutor::GppExecutor;
    
      const char* get_standard();
    };
  
  #endif // HAS_GPP_98
  
  #if HAS_GPP_03
    
    class Gpp03Executor : public GppExecutor {
      using GppExecutor::GppExecutor;
    
      const char* get_standard();
    };
    
  #endif // HAS_GPP_03
  
  #if HAS_GPP_11
  
    class Gpp11Executor : public GppExecutor {
      using GppExecutor::GppExecutor;
    
      const char* get_standard();
    };
    
  #endif // HAS_GPP_11
  
  #if HAS_GPP_14
  
    class Gpp14Executor : public GppExecutor {
      using GppExecutor::GppExecutor;
    
      const char* get_standard();
    };
    
  #endif // HAS_GPP_14
  
  #if HAS_GPP_17
  
    class Gpp17Executor : public GppExecutor {
      using GppExecutor::GppExecutor;
    
      const char* get_standard();
    };
    
  #endif // HAS_GPP_17

#endif // HAS_GPP