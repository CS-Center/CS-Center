#include "cpython_executor.hpp"

#if HAS_CPYTHON

  void CPythonExecutor::make_base_args() {
    args = { get_exec_name(), "-BS", source_filename.c_str() };
  }
  
  const char* CPythonExecutor::get_source_ext() { return ".py"; }
  
  #if HAS_CPYTHON_2
  
    const char* CPython2Executor::get_exec() { return CPYTHON2_PATH; }
    const char* CPython2Executor::get_exec_name() { return "python2"; }
    
  #endif // HAS_CPYTHON_2
  
  #if HAS_CPYTHON_3
  
    const char* CPython3Executor::get_exec() { return CPYTHON3_PATH; }
    const char* CPython3Executor::get_exec_name() { return "python3"; }
    
  #endif // HAS_CPYTHON_3
  
#endif // HAS_CPYTHON