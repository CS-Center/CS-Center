#include "executors/config/python.hpp"

#if HAS_PYPY

  #include "executors/executor.hpp"

  class PyPyExecutor : public Executor {
    using Executor::Executor;
    
    void make_base_args();  
    
    const char* get_source_ext();
    
    virtual const char* get_exec() = 0;
    virtual const char* get_exec_name() = 0;
  };
  
  #if HAS_PYPY_2
  
    class PyPy2Executor : public PyPyExecutor {
      using PyPyExecutor::PyPyExecutor;
      
      const char* get_exec();
      const char* get_exec_name();
    };
    
  #endif // HAS_PYPY_2
  
  #if HAS_PYPY_3
  
    class PyPy3Executor : public PyPyExecutor {
      using PyPyExecutor::PyPyExecutor;
      
      const char* get_exec();
      const char* get_exec_name();
    };
    
  #endif // HAS_PYPY_3

#endif // HAS_PYPY