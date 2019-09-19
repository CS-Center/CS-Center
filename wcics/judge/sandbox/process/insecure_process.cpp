#include <stdio.h>
#include <sys/wait.h>

#include "insecure_process.hpp"
#include "utils/time.hpp"

int InsecureProcess::monitor() {
  struct timespec tv, total, start, end, delta;
  total.tv_sec = total.tv_nsec = 0;
  
  // delay is .1 seconds
  // process may not get a fully fair timelimit because of this delay, but i dont really care
  tv.tv_sec = 0;
  tv.tv_nsec = 1e8;
  
  pid_t ret;
  int status;
  bool tle = false;
  
  do {
    clock_gettime(CLOCK_MONOTONIC, &start);
    
    if(nanosleep(&tv, NULL)) {
      perror("InsecureProcess::start_monitor: nanosleep");
    }
    
    clock_gettime(CLOCK_MONOTONIC, &end);
    timespec_sub(end, start, delta);
    timespec_add(total, delta, total);
    
    if(total.tv_sec + (total.tv_nsec / 1e9) > pconf.timelimit) {
      tle = true;
      break;
    }
  
    ret = waitpid(pid, &status, WNOHANG);
  } while(ret == 0);
  
  if(tle) {
    terminate();
    
    res.death_type = DEATH_TLE;
    
    return 0;
  }
  
  if(WIFEXITED(status)) {
    res.death_type = DEATH_NORMAL;
    res.exit_info = WEXITSTATUS(status);
  }
  
  if(WIFSIGNALED(status)) {
    res.death_type = DEATH_SIGNAL;
    res.exit_info = WTERMSIG(status);
  }
  
  return 0;
}

void InsecureProcess::child_func() {}

InsecureProcess::InsecureProcess(
  AsyncCommunicator& comm,
  const char* file, 
  const char* const* args, 
  const char* const* env, 
  config& conf,
  SharedProcessResult& pres
) : 
  Process(comm, file, args, env, conf, pres)
{}