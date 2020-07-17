#include "cpython_executor.hpp"

#if defined PYTHON_ENABLED && defined CPYTHON_ENABLED

  void CPythonExecutor::make_base_args() {
    args = { get_exec_name(), "-BS", source_filename.c_str() };
  }
  
  const char* CPythonExecutor::get_source_ext() { return ".py"; }
  
  #ifdef CPYTHON2_ENABLED
  
    const char* CPython2Executor::get_exec() { return CPYTHON2_PATH; }
    const char* CPython2Executor::get_exec_name() { return "python2"; }
    
  #endif // CPYTHON2_ENABLED
  
  #ifdef CPYTHON3_ENABLED
  
    const char* CPython3Executor::get_exec() { return CPYTHON3_PATH; }
    const char* CPython3Executor::get_exec_name() { return "python3"; }
    
  #endif // CPYTHON3_ENABLED
  
#endif // PYTHON_ENABLED && CPYTHON_ENABLED