#include "java_executor.hpp"

#if HAS_JAVA
  #include <string.h>

  const char* JavaExecutor::get_compiler_name() { return "javac"; }
  
  const char* JavaExecutor::get_ext() { return "java"; }
    
  const char* JavaExecutor::get_exec() { return JAVA_VM_PATH; }

  int JavaExecutor::create_files() {
    int flen = strlen(file);
    strncpy(classfile, file, FILE_MAX_LEN);
    strncpy(classfile + flen, ".class", FILE_MAX_LEN - flen);
    
    return CompiledExecutor::create_files();
  }
  
  const char* JavaExecutor::get_output_file() {
    return classfile;
  }

  int JavaExecutor::get_nproc() { return -1; }

  int JavaExecutor::make_args() {
    // Initial heap of 1 MB, use serial GC, and dont log perf data
    const char* base_args[] = {
      "java", "-Xms1m", "-XX:+UseSerialGC", "-XX:-UsePerfData", file, 0
    };
    
    return add_args(base_args);
  }

  #if HAS_JAVA_8
  
    const char* Java8Executor::get_compiler() { return JAVA_8_COMPILER_PATH; }
  
  #endif // HAS_JAVA_8
  
  #if HAS_JAVA_11
  
    const char* Java11Executor::get_compiler() { return JAVA_11_COMPILER_PATH; }
  
  #endif // HAS_JAVA_111

#endif // HAS_JAVA