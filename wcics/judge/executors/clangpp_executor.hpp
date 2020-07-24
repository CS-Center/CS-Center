#pragma once

#include <string>
#include "executors/config/cpp.hpp"

#if HAS_CLANGPP

  #include "executors/compiled_executor.hpp"

	template<int std>
  class ClangppExecutor : public CompiledExecutor {
    using CompiledExecutor::CompiledExecutor;
  
		std::string std_flag;
	
		const char* get_standard() {
			std_flag = "-std=c++" + to_string(std);
		}
  
    std::vector<const char*> get_compiler_flags() {
			return {
				"-O", "-Wall", "-DONLINE_JUDGE", get_standard(), "-o", compiled_filename.c_str(),
			};
		}
  
    const char* get_compiler_exec() { return CLANGPP_PATH; }
    const char* get_compiler_name() { return "clang++"; }
    
    const char* get_ext() { return ".cpp"; }
  };

#endif // HAS_CLANGPP