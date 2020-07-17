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
#include "utils/debug.hpp"

Process::Process(const char* file, const char* const* args, const char* const* env, config& conf, SharedProcessResult& pr) :
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

void Process::fork_and_exec() {      
  pid = fork();
  if(pid == -1) {
    res.death_ie("Process: launch: fork");
    RUNTIME_FUNC(-1);
  }
  if(pid == 0) {  
    // child
    pconf.init(res);
            
    child_func();
    
    execve(pathname, const_cast<char* const*>(argv), const_cast<char* const*>(envp));
        
    res.death_ie("Process: launch: execve");
        
    _exit(-1);
  }
}

void Process::launch() {
  fork_and_exec();
  monitor();
} 

const process_result& Process::get_result() {
  return res;
}