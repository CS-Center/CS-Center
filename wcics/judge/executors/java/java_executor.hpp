#include "runtimes/java.hpp"

#if JAVA_ENABLED

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
  
  #if JAVA_8_ENABLED
  
    class Java8Executor : public JavaExecutor {
      using JavaExecutor::JavaExecutor;
    
      const char* get_compiler();
    };
  
  #endif // JAVA_8_ENABLED
  
  #if JAVA_11_ENABLED
  
    class Java11Executor : public JavaExecutor {
      using JavaExecutor::JavaExecutor;
    
      const char* get_compiler();
    };
  
  #endif // JAVA_11_ENABLED

#endif // JAVA_ENABLED