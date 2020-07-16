#include <errno.h>
#include <signal.h>
#include <time.h>

#include "secure_process_shocker.hpp"
#include "secure_process.hpp"

#include "utils/debug.hpp"

// TODO: make res.error use a futex?
void SecureProcessShocker::launch() {
  struct timespec delay;
  delay.tv_sec = 0;
  delay.tv_nsec = 1e8;

  while(1) {
    nanosleep(&delay, 0);
        
    // send SIGWINCH
    if(killpg(proc.pid, SIGWINCH)) {
      if(errno == ESRCH)
        return;
        
      proc.death_ie("SecureProcess::shocker_main: kill");      
      RUNTIME_FUNC(-1);
    }
  }
}

SecureProcessShocker::SecureProcessShocker(SecureProcess& proc) : proc(proc) {}