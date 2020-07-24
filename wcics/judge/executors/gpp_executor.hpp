#pragma once

#include <string>
#include "executors/config/cpp.hpp"

#if HAS_GPP

  #include "executors/compiled_executor.hpp"

	template<int std>
  class GppExecutor : public CompiledExecutor {
    using CompiledExecutor::CompiledExecutor;
  
    const char* get_standard() {
			std_flag = "-std=c++" + std::to_string(std);
			
			return std_flag.c_str();
		}
  
    std::vector<const char*> get_compiler_flags() {
			return {
				"-O", "-Wall", "-DONLINE_JUDGE", "-o", compiled_filename.c_str(), get_standard()
			};
		}
  
    const char* get_compiler_exec() { return GPP_PATH; }
    const char* get_compiler_name() { return "g++"; }
    
    const char* get_source_ext() { return ".cpp"; }
  };

#endif // HAS_GPP