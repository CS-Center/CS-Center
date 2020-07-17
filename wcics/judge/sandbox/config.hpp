#pragma once

#include "process_result.hpp"

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
  
  // streams
  // -1 indicates stream inheritance
  int pstdin, pstdout, pstderr;
  
  config();
  
  void set_streams(int, int, int);
  
  void init(process_result&);
};