#pragma once

#include <vector>

#include "executor.hpp"

typedef Executor* (*get_executor_sig) (const char* code, const char* file, const char* const* extra_args, const char* const* env, config&, AsyncCommunicator&, FileAccessChecker&, SharedProcessResult& res);

#define EXECUTOR_INFO_LEN 4096

class ExecutorInfo {
public:
  const char* exec;

  // full display name, e.g. Python 3
  const char* const fullname;
  
  // identifying name, shorter, e.g. cpy3, pypy3 or gcc90
  const char* const shortname;

  // language name
  const char* const language;
  
  // major language version
  const int major_version;
  
  // name of runtime e.g. CPython, GCC
  const char* const runtime;
  
  const char* const* const version_args;
  
  // get a new executor
  get_executor_sig make_executor;
  
  // whether or not they use stderr for their version output
  bool stderr_version;
  
  bool has_version = false;
  
  char info[EXECUTOR_INFO_LEN + 1];
  
  const char* get_info();
  
  ExecutorInfo(const char* exec, const char* fullname, const char* shortname, const char* lang, int version, const char* runtime, const char* const* args, bool, get_executor_sig);
};

extern std::vector<ExecutorInfo> executors;