#include "executors/config/java.hpp"

#if HAS_JAVA

  #include "executors/compiled_executor.hpp"

  class JavaExecutor : public CompiledExecutor {
    using CompiledExecutor::CompiledExecutor;

    char classfile[FILE_MAX_LEN + 1];
    
    int create_files() override;

    const char* get_ext();
    
    int get_nproc() override;
    
    const char* get_output_file() override;
    
    // compiler is determined by version, but name is always 'javac'
    const char* get_compiler() = 0;
    const char* get_compiler_name();
    
    int make_args();
        
    // always run on the same VM, since it can run older versions of code
    const char* get_exec();
  };
  
  #if HAS_JAVA_8
  
    class Java8Executor : public JavaExecutor {
      using JavaExecutor::JavaExecutor;
    
      const char* get_compiler();
    };
  
  #endif // HAS_JAVA_8
  
  #if HAS_JAVA_11
  
    class Java11Executor : public JavaExecutor {
      using JavaExecutor::JavaExecutor;
    
      const char* get_compiler();
    };
  
  #endif // HAS_JAVA_11

#endif // HAS_JAVA