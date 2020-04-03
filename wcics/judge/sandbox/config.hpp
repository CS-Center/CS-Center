#pragma once

#include "process_result.hpp"

#include "utils/scoped_fd.hpp"

// A structure for a process config
struct config {
  // RLIMIT_DATA & RLIMIT_STACK
  long memory;
  
  // this is monitored in a different way
  double timelimit;
  
  // RLIMIT_CORE
  int core;
  
  // RLIMIT_MEMLOCK
  long memlock;

  // RLIMIT_FSIZE
  long fsize;
  
  // RLIMIT_NPROC
  int nproc;
  
  // Dir to chdir into
  const char* dir;
  
  config();
  
  void init(process_result&);
};

// since this has to be redone every time the process is run, this is a separate structure
struct file_config {
  // streams
  // -1 indicates stream inheritance
  int pstdin, pstdout, pstderr;
  
  file_config(int in, int out, int err);
  
  // initiailize
  void init(process_result&);
};