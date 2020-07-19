#include "runtimes/python.hpp"

#if PYTHON_ENABLED && PYPY_ENABLED

  #include "executors/executor.hpp"

  class PyPyExecutor : public Executor {
    using Executor::Executor;
    
    void make_base_args();  
    
    const char* get_source_ext();
    
    virtual const char* get_exec() = 0;
    virtual const char* get_exec_name() = 0;
  };
  
  #if PYPY2_ENABLED
  
    class PyPy2Executor : public PyPyExecutor {
      using PyPyExecutor::PyPyExecutor;
      
      const char* get_exec();
      const char* get_exec_name();
    };
    
  #endif // PYPY2_ENABLED
  
  #if PYPY3_ENABLED
  
    class PyPy3Executor : public PyPyExecutor {
      using PyPyExecutor::PyPyExecutor;
      
      const char* get_exec();
      const char* get_exec_name();
    };
    
  #endif // PYPY3_ENABLED

#endif // PYTHON_ENABLED & PYPY_ENABLED