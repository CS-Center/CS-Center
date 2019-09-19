#include "pypy_executor.hpp"

#if defined PYTHON_ENABLED && defined PYPY_ENABLED

  int PyPyExecutor::make_args() {
    const char* base_args[] = { get_exec_name(), "-BS", filepath, 0 };
    
    return add_args(base_args);
  }

  const char* PyPyExecutor::get_ext() {
    return "py";
  }
  
  #ifdef PYPY2_ENABLED
  
    const char* PyPy2Executor::get_exec() { return PYPY2_PATH; }
    
    const char* PyPy2Executor::get_exec_name() { return "pypy2"; }
  
  #endif // PYPY2_ENABLED
  
  #ifdef PYPY3_ENABLED
  
    const char* PyPy3Executor::get_exec() { return PYPY3_PATH; }
    
    const char* PyPy3Executor::get_exec_name() { return "pypy3"; }
  
  #endif // PYPY3_ENABLED

#endif // PYTHON_ENABLED && PYPY_ENABLED