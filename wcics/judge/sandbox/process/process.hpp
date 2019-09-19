#pragma once

#include <sys/types.h>

#include "sandbox/process_result.hpp"
#include "sandbox/config.hpp"
#include "sandbox/communicator.hpp"

// monitoring thread does not need a large stack, so this is fine
#define MONITOR_STACK_SIZE (1 << 16)

// A simple abstract base to have some of the basic process attributes and methods
class Process {
protected:
  AsyncCommunicator& comm;

  // variables to be passed to execve
  const char* pathname;
  const char* const *argv;
  const char* const *envp;
  
  // The process pid, only available after calling launch()
  pid_t pid;
  
  // The process_result
  process_result& res;
  
  // Config
  config& pconf;
  
  // kill the process
  void terminate();
  
  // fork and exec the process
  int fork_and_exec();
  
  // monitor process
  virtual int monitor() = 0;
  
  // do initialization in the child
  virtual void child_func() = 0;
  
public:
  Process(
    AsyncCommunicator& comm,
    const char* file, 
    const char* const* args, 
    const char* const* env, 
    config& conf,
    SharedProcessResult& pres
  );
  
  // launch the process
  int launch();
  
  const process_result& get_result();
};