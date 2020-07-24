#pragma once

#include <vector>

#include "executor.hpp"

typedef Executor* (*get_executor_sig) (EXEC_INIT_ARGS);

class ExecutorInfo {
public:
  // full display name, e.g. Python 3
  const char* const fullname;
  
  // identifying name, shorter, e.g. cpy3, pypy3 or gcc90
  const char* const shortname;

  // language name
  const char* const language;
  
  // name of runtime e.g. CPython, GCC
  const char* const runtime;
  
  // major language version
  const int major_version;
      
  std::string info;
	
  ExecutorInfo(const char* exec, const char* fullname, const char* shortname, int version, bool, const char* vflag, const char* runtime, const char* lang, get_executor_sig);
};

extern std::vector<ExecutorInfo> executors;