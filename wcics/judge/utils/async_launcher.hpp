#pragma once

#include <sched.h>
#include <sys/types.h>
#include <stdio.h>
#include <linux/futex.h>
#include <errno.h>
#include <sys/syscall.h>
#include <unistd.h>

#include "utils/debug.hpp"

#define MONITOR_STACK_SIZE (1 << 16)

template<typename T>
class AsyncLauncher {
  T& obj;
  pid_t ctid, tid;
  bool started;
  
  char stack[MONITOR_STACK_SIZE];

public:
  AsyncLauncher(T& o) : obj(o), started(false) {}
  
  static void start(void* arg) {
    RUNTIME_FUNC(syscall(SYS_exit, ((T*) arg)->launch()));
  }
  
  void monitor() {
    tid = RUNTIME_FUNC(
    
      clone(AsyncLauncher<T>::start, stack + sizeof(stack),
        CLONE_CHILD_CLEARTID | CLONE_CHILD_SETTID | CLONE_FILES | CLONE_FS | CLONE_IO |
        CLONE_PARENT_SETTID | CLONE_SIGHAND | CLONE_THREAD | CLONE_VM,
        &obj, &ctid, 0, &ctid)
        
    );
            
    started = true;
  }
  
  void wait() {
    if(!started) {
      fputs("Warning: Not waiting for monitoring thread, thread not started!\n", stderr);
      return;
    }
  
    int r = syscall(SYS_futex, &ctid, FUTEX_WAIT, tid, 0);
    
    if(r < 0 && errno != EAGAIN) RUNTIME_FUNC(-1);
  }
  
  T& get_obj() { return obj; }
};