#pragma once

#include <limits.h>
#include <string>
#include <vector>

#include "sandbox/config.hpp"
#include "sandbox/process/secure_process/secure_process.hpp"
#include "sandbox/file-access-checker/file_access_checker.hpp"
#include "sandbox/process_result.hpp"

#include "executor_utils.hpp"

class Executor {
protected:
  // whether or not cleanup has occurred yet
  bool has_cleanup = false;

  // the file
  const char* file;
  
  std::string code;
  
  // args array, filled after prepare()
  std::vector<const char*> args;
    
  // the extra args and env
  std::vector<const char*> extra_args;
  const char* const* env;
  
  // config
  config& conf;
  
  // fac
  FileAccessChecker& fac;
  
  // proc result
  SharedProcessResult& res;
  
  // the executable
  std::string exec;
  
  // the full path and ext of file
  std::string filepath;
  
  // make arguments
  virtual void make_args();
  
  // get executable path and name
  virtual const char* get_exec();
  virtual const char* get_exec_name();
  
  // get nproc (default 0)
  virtual int get_nproc();
  
  virtual process_result* get_compiler_result();
  
  // getters
  virtual const char* get_ext();
  
  virtual void create_files();
  virtual void compile();
  
public:
  // file is the file name to put the code in
  Executor(std::string code, std::string file, std::vector<const char*> extra_args, const char* const* env, config&, FileAccessChecker&, SharedProcessResult& res);

  // prepare for launching
  // this _MUST_ be called before launch, and launch can be called many times after this is called
  virtual void prepare();
  
  // get the compiler error
  const char* get_compiler_info();
  
  // called for cleanup
  virtual void cleanup();

  virtual void launch();
}