#pragma once

#include <string>
#include <vector>

#include "sandbox/config.hpp"
#include "sandbox/file-access-checker/file_access_checker.hpp"
#include "sandbox/process_result.hpp"

class executor_config;
struct econf_str;

class Executor {
private:
	bool has_cleanup = false;

  std::string code;

  const char* const* env;

	config& conf;
  FileAccessChecker& fac;
  SharedProcessResult& res;
	executor_config& ec;

  std::string base_filename;
  std::string source_filename;
	std::string source_filepath;
	std::string compiled_filename;
	std::string compiled_filepath;

	SharedProcessResult compiler_res;
	std::string compiler_output;

	std::vector<const char*> args;

	const char* get_str(econf_str&);

	bool is_compiled();

	void compile();

	config get_compiler_config(int);

public:
  // file is the file name to put the code in
  Executor(executor_config&, std::string code, const char* file, std::vector<const char*> extra_args, const char* const* env, config& conf, FileAccessChecker& fac, SharedProcessResult& res);

  // called for cleanup
  void cleanup();

  void launch();

	process_result& get_result();

	~Executor();
};
