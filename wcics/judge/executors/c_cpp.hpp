#pragma once

#include <vector>

struct executor_config;

struct c_cpp_config {
	const char* id;
	const char* fullname;
	const char* stdflag;
	const char* major_version;

	bool enabled;

	update(executor_config&);
};

extern std::vector<c_cpp_config> gcc_confs;
extern std::vector<c_cpp_config> gpp_confs;
extern std::vector<c_cpp_config> clang_confs;
extern std::vector<c_cpp_config> clangpp_confs;
