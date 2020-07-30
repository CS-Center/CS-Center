#pragma once

#include <vector>

#include "executor.hpp"

namespace econf {
	enum {
		null,
		no_compiler,
		base_file, // Base filename as passed to the executor
		
		source_filename, // base_file + source_ext
		source_filepath, // dir + source_name
		
		compiled_filename, // base_file + compiled_ext
		compiled_filepath, // dir + compiled_name
		
		err,
	};
}

// String / Special value ^^
struct econf_str {
	std::string str;
	int val; // one of the constants from econf enum
	
	econf_str(int x);
	econf_str(const char* s);
};

// Enough info to construct an ExecutorInfo

struct executor_config {
	// compiler arguments
	econf_str compiler_exec;
	std::vector<econf_str> compiler_args; // passed directly to execve(2)
	std::string source_ext;
	
	// interpreter arguments
	econf_str interpreter_exec;
	std::vector<econf_str> interpreter_args; // add the "extra_args" 
	std::string compiled_ext;
	
	// compiler limits
	int compiler_memory = 128 * 1024 * 1024;
	double compiler_timelimit = 10;
	int compiler_max_file_size = 64 * 1024 * 1024;
	
	// identifying name, cpy3, pypy3 or gcc90
	std::string id;
	
	// Fullname ("C++03 - G++", "Python2 - CPython")
	std::string fullname;
	
	// Language name/ID
	std::string language_name, language_id;
	
	// runtime name/ID
	std::string runtime_name, runtime_id;
	
	// major version number
	std::string version;
	

	int nproc = 0;
};

class ExecutorFactory {
public:
	// copy everything, intentionally not a reference
  executor_config ec;
	
	template<typename... targs>
	Executor create(targs... args) {
		return Executor(ec, args...);
	}
	
  ExecutorFactory(executor_config ec);
};

extern std::vector<ExecutorFactory> executor_factories = get_factories();

ExecutorFactory& get_factory(const char* id);