#include <stdio.h>
#include <sys/wait.h>

#include "insecure_process.hpp"
#include "utils/time.hpp"

void InsecureProcess::monitor() {
  struct timespec tv, total, start, end, delta;
  total.tv_sec = total.tv_nsec = 0;
  
  // delay is .5 seconds
  // process may not get a fully fair timelimit because of this delay, but i dont really care
  tv.tv_sec = 0;
  tv.tv_nsec = 5e8;
  
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
    
    return;
  }
  
  if(WIFEXITED(status)) {
    res.death_type = DEATH_NORMAL;
    res.exit_info = WEXITSTATUS(status);
  }
  
  if(WIFSIGNALED(status)) {
    res.death_type = DEATH_SIGNAL;
    res.exit_info = WTERMSIG(status);
  }
}

void InsecureProcess::child_func() {}