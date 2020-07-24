#pragma once

#include "executors/config/python.hpp"

#if HAS_CPYTHON

  #include "executors/executor.hpp"

  class CPythonExecutor : public Executor {
    using Executor::Executor;
    
    void make_base_args();
    
    const char* get_source_ext();
    
    // make it abstract :)
    const char* get_exec() = 0;
    const char* get_exec_name() = 0;
  };
  
  #if HAS_CPYTHON_2
  
    class CPython2Executor : public CPythonExecutor {
      using CPythonExecutor::CPythonExecutor;
      
      const char* get_exec();
      const char* get_exec_name();
    };
  
  #endif // HAS_CPYTHON_2
  
  #if HAS_CPYTHON_3
  
    class CPython3Executor : public CPythonExecutor {
      using CPythonExecutor::CPythonExecutor;
      
      const char* get_exec();
      const char* get_exec_name();
    };
  
  #endif // HAS_CPYTHON_3

#endif // HAS_CPYTHON