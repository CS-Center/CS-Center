#pragma once

#include "sandbox/config.hpp"
#include "sandbox/process_result.hpp"

#include "executor.hpp"

class CompiledExecutor : public Executor {
  using Executor::Executor;

protected:
  bool compiled = false;
  
  std::string compiled_filename, compiled_filepath;
  
  SharedProcessResult comp_res;
  std::string compiler_output;

  std::vector<const char*> cargs;
  
  void make_compiler_config(config& cconf);
  
  virtual double get_compiler_timelimit();
  virtual int get_max_file_size();

  virtual const char* get_compiler_exec() = 0;
  virtual const char* get_compiler_name() = 0;

  virtual void make_compiler_args();
  virtual std::vector<const char*> get_compiler_flags();
  
  virtual process_result* get_compiler_result();
  virtual std::string get_compiler_output();
  
  virtual const char* get_compiled_ext();
  
public:
  virtual void compile();
  
  virtual void cleanup();
  
  ~CompiledExecutor();
};