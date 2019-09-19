#pragma once

#include "sandbox/config.hpp"
#include "sandbox/process_result.hpp"

#include "executor.hpp"

class CompiledExecutor : public Executor {
  using Executor::Executor;

protected:
  bool compiled = false;

  arg_arr cargs;
  int cargi = 0;
  
  void make_compiler_config(config& cconf);
  
  virtual double get_compiler_timelimit();
  virtual int get_max_file_size();

  virtual const char* get_compiler() = 0;
  virtual const char* get_compiler_name() = 0;

  virtual int copy_compiler_args(const char**);
  virtual int make_compiler_args();
  
  virtual const char* get_output_file();
  
public:
  virtual int compile();
  
  virtual int cleanup();
  
  ~CompiledExecutor();
};