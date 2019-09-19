#pragma once

#include <sched.h>
#include <sys/types.h>
#include <stdio.h>
#include <linux/futex.h>
#include <errno.h>
#include <sys/syscall.h>
#include <unistd.h>

#define MONITOR_STACK_SIZE (1 << 16)

template<typename T>
class AsyncLauncher {
  T& obj;
  pid_t ctid, tid;
  bool started;
  
  char stack[MONITOR_STACK_SIZE];

public:
  AsyncLauncher(T& o) : obj(o), started(false) {}
  
  static int start(void* arg) {
    syscall(SYS_exit, ((T*) arg)->launch());
    perror("Could not exit from monitoring thread!");
    return -1;
  }
  
  int monitor() {
    tid = clone(AsyncLauncher<T>::start, stack + sizeof(stack),
      CLONE_CHILD_CLEARTID | CLONE_CHILD_SETTID | CLONE_FILES | CLONE_FS | CLONE_IO |
      CLONE_PARENT_SETTID | CLONE_SIGHAND | CLONE_THREAD | CLONE_VM,
      &obj, &ctid, 0, &ctid);
      
    if(tid == -1)
      return -1;
      
    started = true;
      
    return 0;
  }
  
  int wait() {
    if(!started) {
      fputs("Warning: Not waiting for monitoring thread, thread not started!\n", stderr);
      return 0;
    }
  
    if(syscall(SYS_futex, &ctid, FUTEX_WAIT, tid, 0) && errno != EAGAIN)
      return -1;
    
    return 0;
  }
  
  T& get_obj() { return obj; }
};