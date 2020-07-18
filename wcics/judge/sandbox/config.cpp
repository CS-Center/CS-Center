#include <sys/resource.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <utility>
#include <string>

#include "utils/pipes.hpp"
#include "config.hpp"
#include "utils/debug.hpp"

using namespace std;

config::config() :
  memory(64 * 1024 * 1024),
  timelimit(1),
  core(0),
  memlock(0),
  fsize(0),
  nproc(0),
  dir(0),
  pstdin(-1),
  pstdout(-1),
  pstderr(-1)
{}

inline void _setr(int res, struct rlimit& rlim, rlim_t val, const char* name, process_result& pres) { 
  rlim.rlim_cur = rlim.rlim_max = val; 
  if(setrlimit(res, &rlim)) { 
    string out = string("Error setting resource ") + name + " in _setr (child)";
    
    pres.death_ie(out.c_str());
    _exit(-1);
  }
}

#define setr(res, rlim, val, pres) _setr(res, rlim, val, #res, pres)

inline void _movefd(int oldfd, int newfd, const char* nm, process_result& pres) {
  // don't close oldfd, we assume that it is marked Close-on-exec, as it should be
  if(oldfd != -1 && dup2(oldfd, newfd)) {
    string out = string("Error setting child fd '") + nm + "'";
    
    pres.death_ie(out.c_str());
    _exit(-1);
  }
}

#define movefd(old, new, pres) _movefd(old, new, #old, pres)

// if any init fails, kill the process
void config::init(process_result& res) {
  struct rlimit rlim;
  
  setr(RLIMIT_DATA, rlim, memory, res);
  
  setr(RLIMIT_STACK, rlim, memory, res);
  
  setr(RLIMIT_FSIZE, rlim, fsize, res);
  
  if(nproc != -1)
    setr(RLIMIT_NPROC, rlim, nproc, res);
  
  setr(RLIMIT_CORE, rlim, core, res);
      
  if(dir && chdir(dir)) {
    res.death_ie("config::init: chdir");
    _exit(-1);
  }
  
  movefd(pstdin, 0, res);
  
  movefd(pstdout, 1, res);
  
  movefd(pstderr, 2, res);
}

void config::set_streams(int in, int out, int err) {
  pstdin = in;
  pstdout = out;
  pstderr = err;
}