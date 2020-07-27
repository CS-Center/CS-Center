#pragma once

#include <string>
#include "executors/config/c.hpp"

#if HAS_GCC

  #include "executors/compiled_executor.hpp"

	template<int std>
  class GCCExecutor : public CompiledExecutor {
    using CompiledExecutor::CompiledExecutor;
  
		std::string std_flag;
	
    const char* get_standard() {
			std_flag = "-std=c++" + std::to_string(std);
			
			return std_flag.c_str();
		}
    
    std::vector<const char*> get_compiler_flags() {
			return {
				"-Wall", "-DONLINE_JUDGE", "-O", get_standard(), "-o", compiled_filename.c_str()
			};
		}
  
    const char* get_compiler_exec() { return GCC_PATH; } 
    const char* get_compiler_name() { return "gcc"; }
    
    const char* get_source_ext() { return ".c"; }
  };

#endif // HAS_GCC