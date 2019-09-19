#include "runtimes/python.hpp"

#if defined PYTHON_ENABLED && defined PYPY_ENABLED

  #include "executors/executor.hpp"

  class PyPyExecutor : public Executor {
    using Executor::Executor;
    
    int make_args();  
    
    const char* get_ext();
    
    virtual const char* get_exec() = 0;
    virtual const char* get_exec_name() = 0;
  };
  
  #ifdef PYPY2_ENABLED
  
    class PyPy2Executor : public PyPyExecutor {
      using PyPyExecutor::PyPyExecutor;
      
      const char* get_exec();
      const char* get_exec_name();
    };
    
  #endif // PYPY2_ENABLED
  
  #ifdef PYPY3_ENABLED
  
    class PyPy3Executor : public PyPyExecutor {
      using PyPyExecutor::PyPyExecutor;
      
      const char* get_exec();
      const char* get_exec_name();
    };
    
  #endif // PYPY3_ENABLED

#endif // PYTHON_ENABLED & PYPY_ENABLED