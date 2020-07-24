#pragma once

#include <string>
#include "executors/config/c.hpp"

#if HAS_CLANG

  #include "executors/compiled_executor.hpp"

	template<int std>
  class ClangExecutor : public CompiledExecutor {
    using CompiledExecutor::CompiledExecutor;
  
		std::string std_flag;
	
    const char* get_standard() {
			std_flag = "-std=c++" + std::to_string(std);
			
			return std_flag.c_str();
		}
  
    std::vector<const char*> get_compiler_flags() {
			return {
				"-O", "-Wall", "-DONLINE_JUDGE", get_standard(), "-o", compiled_filename.c_str()
			};
		}
  
    const char* get_compiler_exec() { return CLANG_PATH; }
    const char* get_compiler_name() { return "clang"; }
    
    const char* get_source_ext() { return ".c"; }
  };

#endif // HAS_CLANG