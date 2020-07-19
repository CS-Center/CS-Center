#include "pypy_executor.hpp"

#if PYTHON_ENABLED && PYPY_ENABLED

  void PyPyExecutor::make_base_args() {
    args = { get_exec_name(), "-BS", source_filename.c_str() };
  }

  const char* PyPyExecutor::get_source_ext() {
    return ".py";
  }
  
  #if PYPY2_ENABLED
  
    const char* PyPy2Executor::get_exec() { return PYPY2_PATH; }
    
    const char* PyPy2Executor::get_exec_name() { return "pypy2"; }
  
  #endif // PYPY2_ENABLED
  
  #if PYPY3_ENABLED
  
    const char* PyPy3Executor::get_exec() { return PYPY3_PATH; }
    
    const char* PyPy3Executor::get_exec_name() { return "pypy3"; }
  
  #endif // PYPY3_ENABLED

#endif // PYTHON_ENABLED && PYPY_ENABLED