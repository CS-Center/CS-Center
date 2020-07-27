#include <vector>

#include "c_cpp.hpp"
#include "executor_info.hpp"
#include "utils/debug.hpp"

// ExecutorInfo impl
ExecutorInfo::ExecutorInfo(executor_config ec) : ec(ec) {}

econf_str::econf_str(int x) : val(x), str(0) {}
econf_str::econf_str(const char* s) : val(econf::null), str(s) {}

std::vector<ExecutorInfo> get_executors() {
	executor_config ec;
	
	#if C_ENABLED
		ec.language_name = "C";
		ec.language_id = "c";
		
		ec.source_ext = ".c";
		ec.compiled_ext = "";
		
		ec.interpreter_exec = econf::compiled_filepath;
		ec.interpreter_args = {};
		
		ec.compiler_args = {"name", "-O", "-o", econf::compiled_filename, "-DONLINE_JUDGE", "-Wall", "std"};
		
		#if GCC_ENABLED
			ec.
		#endif 
		
}

std::vector<ExecutorInfo> executor_infos = get_executors();

ExecutorInfo& get_einfo(const char* id) {
	for(ExecutorInfo& ei : executor_infos) {
		if(id == ei.ec.id) return ei;
	}
	
	RUNTIME_FUNC(-1);
}