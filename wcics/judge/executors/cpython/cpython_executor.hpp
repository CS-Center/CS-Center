#pragma once

#include "runtimes/python.hpp"

#if defined PYTHON_ENABLED && defined CPYTHON_ENABLED

  #include "executors/executor.hpp"

  class CPythonExecutor : public Executor {
    using Executor::Executor;
    
    void make_base_args();
    
    const char* get_source_ext();
    
    // make it abstract :)
    const char* get_exec() = 0;
    const char* get_exec_name() = 0;
  };
  
  #ifdef CPYTHON2_ENABLED
  
    class CPython2Executor : public CPythonExecutor {
      using CPythonExecutor::CPythonExecutor;
      
      const char* get_exec();
      const char* get_exec_name();
    };
  
  #endif // CPYTHON2_ENABLED
  
  #ifdef CPYTHON3_ENABLED
  
    class CPython3Executor : public CPythonExecutor {
      using CPythonExecutor::CPythonExecutor;
      
      const char* get_exec();
      const char* get_exec_name();
    };
  
  #endif // CPYTHON3_ENABLED

#endif // PYTHON_ENABLED && CPYTHON_ENABLED