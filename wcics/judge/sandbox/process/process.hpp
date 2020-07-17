#pragma once

#include <string>
#include <vector>
#include <sys/types.h>

#include "sandbox/process_result.hpp"
#include "sandbox/config.hpp"

// A simple abstract base to have some of the basic process attributes and methods
class Process {
protected:
  // variables to be passed to execve
  const char* pathname;
  const char* const* argv;
  const char* const* envp;
  
  // The process pid, only available after calling launch()
  pid_t pid;
  
  // The process_result
  process_result& res;
  
  // Config
  config& pconf;
  
  // kill the process
  void terminate();
  
  // fork and exec the process
  void fork_and_exec();
  
  // monitor process
  virtual void monitor() = 0;
  
  // do initialization in the child
  virtual void child_func() = 0;
  
public:
  Process(
    const char* file, 
    const char* const* args, 
    const char* const* env, 
    config& conf,
    SharedProcessResult& pres
  );
  
  // launch the process
  void launch();
  
  const process_result& get_result();
};