#include <sys/resource.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>

#include "utils/pipes.hpp"
#include "config.hpp"

config::config() :
  memory(64 * 1024 * 1024),
  timelimit(1),
  core(0),
  memlock(0),
  fsize(0),
  nproc(0),
  dir(0)
{}

inline int setr(int res, struct rlimit& rlim, rlim_t val) { 
  rlim.rlim_cur = rlim.rlim_max = val; 
  if(setrlimit(res, &rlim)) { 
    return -1; 
  }
  return 0;
}

inline int movefd(int oldfd, int newfd) {
  // don't close oldfd, we assume that it is marked Close-on-exec, as it should be
  if(oldfd != -1)
    if(dup2(oldfd, newfd) == -1)
      return -1;
  return 0;
}

// if any init fails, kill the process
void config::init(process_result& res) {
  struct rlimit rlim;
  
  if(setr(RLIMIT_DATA, rlim, memory)) {
    res.death_ie("config::init: RLIMIT_DATA");
    _exit(-1);
  }
  
  if(setr(RLIMIT_STACK, rlim, memory)) {
    res.death_ie("config::init: RLIMIT_STACK");
    _exit(-1);
  }
  
  if(setr(RLIMIT_FSIZE, rlim, fsize)) {
    res.death_ie("config::init: RLIMIT_FSIZE");
    _exit(-1);
  }
  
  if(nproc != -1 && setr(RLIMIT_NPROC, rlim, nproc)) {
    res.death_ie("config::init: RLIMIT_NPROC");
    _exit(-1);
  }
  
  if(setr(RLIMIT_CORE, rlim, core)) {
    res.death_ie("config::init: RLIMIT_CORE");
    _exit(-1);
  }
  
  if(movefd(pstderr, 2)) {
    res.death_ie("config::init: movefd(pstderr)");
    _exit(-1);
  }
      
  if(dir) {
    if(chdir(dir)) {
      res.death_ie("config::init: chdir");
      _exit(-1);
    }
  }
}

file_config::file_config(int in, int out, int err) : pstdin(in), pstdout(out), pstderr(err) {}

file_config::init(process_result& res) {
  if(movefd(pstdin, 0)) {
    res.death_ie("config::init: movefd(pstdin)");
    _exit(-1);
  }
  
  if(movefd(pstdout, 1)) {
    res.death_ie("config::init: movefd(pstdout)");
    _exit(-1);
  }
  
  if(movefd(pstderr, 2)) {
    res.death_ie("config::init: movefd(pstderr)");
    _exit(-1);
  }
}

// cleanup the dupped files
// cant think of any reason for the parent to need to keep them
// if they do, they can dup them
int file_config::parent_cleanup() {
  if(pstdin != -1 && pstdin != null_read_fd) {
    if(close(pstdin)) {
      perror("config::parent_cleanup: Failed to close pstdin");
      return -1;
    }
  }
  
  if(pstdout != -1 && pstdout != null_write_fd) {
    if(close(pstdout)) {
      perror("config::parent_cleanup: Failed to close pstdout");
      return -1;
    }
  }
  
  if(pstderr != -1 && pstderr != null_write_fd) {
    if(close(pstderr)) {
      perror("config::parent_cleanup: Failed to close pstderr");   
      return -1;
    }
  }
  
  return 0;
}