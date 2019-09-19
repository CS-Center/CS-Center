#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <stdio.h>
#include <linux/futex.h>
#include <sys/syscall.h>
#include <errno.h>
#include <sys/wait.h>
#include <sched.h>
#include <stdlib.h>
#include <time.h>

#include "process.hpp"
#include "utils/args.hpp"
#include "utils/time.hpp"

Process::Process(AsyncCommunicator& comm, const char* file, const char* const *args, const char* const *env, config& conf, SharedProcessResult& pr) :
  comm(comm),
  pathname(file),
  argv(args),
  envp(env),
  res(*pr),
  pconf(conf)
{}

void Process::terminate() {
  if(kill(pid, SIGKILL) && errno != ESRCH) {
    res.death_ie("error killing process");
  }
}

int Process::fork_and_exec() {      
  pid = fork();
  if(pid == -1) {
    res.death_ie("Process: launch: fork");
    return -1;
  }
  if(pid == 0) {  
    // child
    pconf.init(res);
        
    child_func();
    
    execve(pathname, const_cast<char* const*>(argv), const_cast<char* const*>(envp));
        
    res.death_ie("Process: launch: execve");
        
    _exit(-1);
  }
  
  if(pconf.parent_cleanup()) {
    fputs("Process::fork_and_exec: failed to cleanup config\n", stderr);
  }
  
  return 0;
}

int Process::launch(void) {
  int status = 0;
  if(!fork_and_exec()) {  
    comm.monitor();
    
    if(monitor())
      status = -1;
    
    comm.wait();
  }
  
  return status;
} 

const process_result& Process::get_result() {
  return res;
}