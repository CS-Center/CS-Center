#include "pypy_executor.hpp"

#if HAS_PYPY

  void PyPyExecutor::make_base_args() {
    args = { get_exec_name(), "-BS", source_filename.c_str() };
  }

  const char* PyPyExecutor::get_source_ext() {
    return ".py";
  }
  
  #if HAS_PYPY_2
  
    const char* PyPy2Executor::get_exec() { return PYPY2_PATH; }
    
    const char* PyPy2Executor::get_exec_name() { return "pypy2"; }
  
  #endif // HAS_PYPY_2
  
  #if HAS_PYPY_3
  
    const char* PyPy3Executor::get_exec() { return PYPY3_PATH; }
    
    const char* PyPy3Executor::get_exec_name() { return "pypy3"; }
  
  #endif // HAS_PYPY_3

#endif // HAS_PYPY