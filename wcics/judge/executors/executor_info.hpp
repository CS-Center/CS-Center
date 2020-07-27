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
		compiled_filepath // dir + compiled_name
	};
}

// String / Special value ^^
struct econf_str {
	const char* str;
	int val; // one of the constants from econf enum
	
	econf_str(int x);
	econf_str(const char* s);
};

// Enough info to construct an ExecutorInfo

struct executor_config {
	econf_str compiler_exec;
	std::vector<econf_str> compiler_args; // passed directly to execve(2)
	const char* source_ext;
	
	econf_str interpreter_exec;
	std::vector<econf_str> interpreter_args; // add the "extra_args" 
	const char* compiled_ext;
	
	int compiler_memory = 128 * 1024 * 1024;
	double compiler_timelimit = 10;
	int compiler_max_file_size = 64 * 1024 * 1024;
	
	// Full display name
	const char* const fullname;
	
	// identifying name, cpy3, pypy3 or gcc90
	const char* const id;
	
	// Language (C, C++, Python)
	const char* const language_name;
	const char* const language_id;
	
	// Runtime (GCC, CPython, OpenJDK)
	const char* const runtime_name;
	const char* const runtime_id;
	
	const int major_version;

	int nproc = 0;
};

class ExecutorInfo {
public:
	// copy everything, intentionally not a reference
  executor_config ec;
	
	template<typename... targs>
	Executor create(targs... args) {
		return Executor(ec, args...);
	}
	
  ExecutorInfo(executor_config ec);
};

extern std::vector<ExecutorInfo> executors;

ExecutorInfo get_einfo(const char* id);