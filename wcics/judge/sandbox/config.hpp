#pragma once

#include "process_result.hpp"

// A structure for a process config
struct config {
  // RLIMIT_DATA & RLIMIT_STACK
  long memory;
  
  // this is monitored in a different way
  double timelimit;
  
  // standard streams, suitable for calls to dup2()
  // pstdin must be readable; pstdout and pstderr must be writable
  // a value of -1 indicates that the file descriptor will not 
     // be changed from its parent
  int pstdin, pstdout, pstderr;
  
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
  
  // 0 on success, -1 on error
  void init(process_result&);
  
  // cleanup the parent context
  int parent_cleanup();
};