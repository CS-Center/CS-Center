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

using namespace std;

Process::Process(string file, vector<string> args, vector<string> env, config& conf, SharedProcessResult& pr) :
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

void Process::fork_and_exec(file_config& file_conf) {      
  pid = fork();
  if(pid == -1) {
    res.death_ie("Process: launch: fork");
    RUNTIME_FUNC(-1);
  }
  if(pid == 0) {  
    // child
    pconf.init(res);
    
    file_conf.init(res);
        
    child_func();
    
    execve(pathname.c_str(), to_charpp(argv), to_charpp(envp));
        
    res.death_ie("Process: launch: execve");
        
    _exit(-1);
  }
}

void Process::launch(file_config& file_conf) {
  fork_and_exec(file_conf);
  monitor();
  
  return status;
} 

const process_result& Process::get_result() {
  return res;
}