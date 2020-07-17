#include <errno.h>
#include <sys/ptrace.h>
#include <sys/prctl.h>
#include <sys/wait.h>
#include <signal.h>
#include <seccomp.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>

#include "sandbox/syscalls/syscalls.hpp"
#include "utils/memory.hpp"
#include "utils/time.hpp"
#include "utils/debug.hpp"
#include "secure_process.hpp"

using namespace std;

// do child initialization
// load the seccomp filter and do other initialization
// if any initialization syscalls fail, kill the process, safety first
void SecureProcess::child_func() {
  // new process group
  // important for threading reading things
  if(setpgid(0, 0)) {
    res.death_ie("SecureProcess::child_func: setpgid");
    _exit(-1);
  }

  // non-dumpable processes cant be attached to, cant have their vm read, etc
  if(prctl(PR_SET_DUMPABLE, 1)) {
    res.death_ie("SecureProcess::child_func: prctl: dumpable");
    _exit(-1);
  }

  // important or something
  // see https://man7.org/linux/man-pages/man2/prctl.2.html
  // or `man 2 prctl`
  if(prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0)) {
    res.death_ie("SecureProcess::child_func: prctl: privs");
    _exit(-1);
  }
  
  // Have the parent trace the child
  if(ptrace(PTRACE_TRACEME, 0, 0, 0)) {
    res.death_ie("SecureProcess::child_func: ptrace");
    _exit(-1);
  }
      
  // signal parent to do initialization
  if(kill(getpid(), SIGWINCH)) {    
    res.death_ie("SecureProcess::child_func: kill");
    _exit(-1);
  }
  
  // load seccomp
  // this is loaded last since the call to load it wont be filtered, but ALL SUBSEQUENT CALLS WILL
  if(seccomp_load(default_seccomp_filter)) {
    res.death_ie("SecureProcess::child_func: seccomp_load");
    _exit(-1);
  }
}

void launch_thread(SecureProcessShocker* arg) {
  arg->launch();
}

// the big boi function
// this is the most important function in the whole judge
void SecureProcess::monitor() {
  shock_thread = thread(launch_thread, &sps);
   
  int status;
  bool first = true;
  
  struct timespec total, delta, pre_wait, post_wait;
  total.tv_sec = total.tv_nsec = 0;
  
  bool has_start = false;
  
  children.insert(pid);
    
  while(!children.empty()) {  
  
    if(has_start) {
      clock_gettime(CLOCK_MONOTONIC, &pre_wait);
    
      // wait on -pid so that the process group is used
      tid = waitpid(-pid, &status, 0);

      clock_gettime(CLOCK_MONOTONIC, &post_wait);
      
      timespec_sub(post_wait, pre_wait, delta);
      timespec_add(total, delta, total);

      res.time = total.tv_sec + total.tv_nsec / 1e9;

      if(total.tv_sec + total.tv_nsec / 1e9 > pconf.timelimit) {
        // TLE
        res.death_tle();

        terminate();
        return;
      }
    }
    
    // since the process hasn't started, we wait on the process, not the pgrp
    // also, we dont track time
    else
      tid = waitpid(pid, &status, 0);
    
    if(tid == -1) {
      // process must already be dead, so just exit
      res.death_ie("SecureProcess::monitor: waitpid");
      return;
    }
            
    // normal death
    if(WIFEXITED(status)) {
      if(res.death_type == NO_DEATH)
        res.death_normal(WEXITSTATUS(status));
        
      children.erase(tid);
    }
    
    // signal death
    else if(WIFSIGNALED(status)) {
      if(res.death_type == NO_DEATH)
        res.death_signal(WTERMSIG(status));
      
      children.erase(tid);
    }
    
    // process stopped
    // they can be stopped for 1234565432345434545 reasons
    else if(WIFSTOPPED(status)) {
      if(first) {        
        // set ptrace options
        ptrace(PTRACE_SETOPTIONS, pid, 0,
          PTRACE_O_TRACEEXEC | 
          PTRACE_O_EXITKILL | // if we die, you go down with us!
          PTRACE_O_TRACEEXIT | // so we can inspect memory and the like
          PTRACE_O_TRACECLONE | // multithreading
          PTRACE_O_TRACEFORK |
          PTRACE_O_TRACEVFORK |
          PTRACE_O_TRACESECCOMP // SECCOMP!
        );

        first = false;
      }
    
      int signal = WSTOPSIG(status);
      // printf("Received signal %d\n", signal);
      
      if(signal == SIGWINCH) {
        // This is our dummy signal
        // so just do nothing lol
        // continue the process
        if(ptrace(PTRACE_CONT, tid, 0, 0) && errno != ESRCH) {
          death_ie("SIGWINCH PTRACE_CONT");
          RUNTIME_FUNC(-1);
        }
        continue;
      }
      
      // If its an event, the event will be returned in status >> 16
      int event = status >> 16;
                  
      // hey seccomp!
      if(event == PTRACE_EVENT_SECCOMP) {
        get_regs();
        
        int call_no = get_syscall();
               
        if(call_no >= MAX_SYSCALL || call_no < 0) {
          update_result(-ENOSYS);
          ptrace(PTRACE_CONT, tid, 0, 0);
          continue;
        }
                        
        switch(syscall_handlers[call_no]) {
          case HANDLER_ERRNO:
          case HANDLER_DENY:
          case HANDLER_SPECIAL:
            death_illegal(call_no);
            return;
            
          case HANDLER_INSPECT: {
            // call inspector gadget
            int ret = RUNTIME_FUNC(syscall_inspectors[call_no](*this));
                        
            // if they returned 0, syscall runs
            // if they returned positive, errno will be returned!
            if(ret > 0) {
              update_result(-ret);
              skip_call();
            }
                        
            set_regs();
            break;
          }
            
          case HANDLER_ALLOW:
          case HANDLER_WHITELIST:
            break;
            
          default:
            string out = string("Invalid syscall handler ") + to_string(syscall_handlers[call_no]) + " for ";
          
            if(syscall_names[call_no])
              out += syscall_names[call_no];
            else
              out += to_string(call_no);
              
            res.death_ie(out.c_str());
            RUNTIME_FUNC(-1);
        }
                
        if(ptrace(PTRACE_CONT, tid, 0, 0) && errno != ESRCH) {
          death_ie("Seccomp PTRACE_CONT");
          RUNTIME_FUNC(-1);
        }
      }
      
      else if(event == PTRACE_EVENT_EXEC) {        
        has_start = true;
        
        if(ptrace(PTRACE_CONT, tid, 0, 0) && errno != ESRCH) {
          death_ie("Exec PTRACE_CONT");
          RUNTIME_FUNC(-1);
        }
      }
      
      // thing is exiting
      else if(event == PTRACE_EVENT_EXIT) {
        res.rss = get_max_rss(tid);
        
        // MLE
        // this really only happens if they try and split their RSS between stack and data
        if(res.rss > pconf.memory)
          res.death_mle();
      
        if(ptrace(PTRACE_CONT, tid, 0, signal) && errno != ESRCH) {
          death_ie("Exit PTRACE_CONT");
          RUNTIME_FUNC(-1);
        }
      }
      
      else if(event == PTRACE_EVENT_CLONE || event == PTRACE_EVENT_FORK || event == PTRACE_EVENT_VFORK) {      
        unsigned long newtid;
        ptrace(PTRACE_GETEVENTMSG, tid, 0, &newtid);
        
        // printf("New thread: %lu\n", newtid);
        children.insert(newtid);
        
        RUNTIME_FUNC(ptrace(PTRACE_CONT, tid, 0, 0));
      }
      
      else {
        // no event
        // this means we have an ACTUAL SIGNAL
        // just send it back
        RUNTIME_FUNC(ptrace(PTRACE_CONT, tid, 0, signal));
      }
    }
  }
}

void SecureProcess::_wait_death() {
  int status;
  while(!children.empty()) {
    // TODO: determine what to wait on based on where we died (pgid or pid)
    
    tid = waitpid(-pid, &status, 0);
    
    if(tid == -1)
      break;
    
    else if(WIFSIGNALED(status)) {
      // terminated
      children.erase(tid);
    }
    
    // this should be the only other event delivered
    else if((status >> 16) == PTRACE_EVENT_EXIT) {
      // continue the process

      // read memory
      res.rss = get_max_rss(tid);
      
      // MLE
      // this really only happens if they try and split their RSS between stack and data
      if(res.rss > pconf.memory)
        res.death_mle();

      if(ptrace(PTRACE_CONT, tid, 0, 0)) {
        // we already sent SIGKILL, so what can we really do if this fails?
        // we print a warning and return
        // this is fine because it should really only give ESRCH
        perror("SecureProcess::_wait_death: ptrace");
        return;
      }
    }
    
    else {
      fprintf(stderr, "Invalid status value in _wait_death: %d\n", status);
      // we dont know what to do here
      // so we just leave
      return;
    }
  }
}