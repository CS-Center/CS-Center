#pragma once

#include <limits.h>

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
  
  const char* code;
  
  // args array, filled after prepare()
  arg_arr args;
  
  int argi = 0;
  
  // the extra args and env
  const char* const* extra_args;
  const char* const* env;
  
  // config
  config& conf;
  
  // fac
  FileAccessChecker& fac;
  
  // proc result
  SharedProcessResult& res;
  
  // the executable
  char exec[FILE_MAX_LEN + 1];
  
  // the full path and ext of file
  char filepath[PATH_MAX + 1];
  
  // the compiler error
  char compiler_info[INFO_BUF_LEN + 1];
  
  // make arguments
  virtual int make_args();
  
  // add to base arguments, and null terminate the array
  virtual int add_args(const char** base);
  
  // get executable path and name
  virtual const char* get_exec();
  virtual const char* get_exec_name();
  
  // get nproc (default 0)
  virtual int get_nproc();
  
  // getters
  virtual const char* get_ext() = 0;
  
  virtual int create_files();
  virtual int compile();
  
public:
  // file is the file name to put the code in
  Executor(const char* code, const char* file, const char* const* extra_args, const char* const* env, config&, FileAccessChecker&, SharedProcessResult& res);

  // prepare for launching
  // this _MUST_ be called before launch, and launch can be called many times after this is called
  virtual int prepare();
  
  // get the compiler error
  const char* get_compiler_info();
  
  // called for cleanup
  virtual int cleanup();

  virtual int launch(file_config&);
  
  void set_env(const char* const*);