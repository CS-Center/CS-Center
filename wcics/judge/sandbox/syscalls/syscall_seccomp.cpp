#include <errno.h>
#include <unistd.h>
#include <seccomp.h>
#include <sched.h>
#include <stdio.h>

#include "syscalls.hpp"

// if any filter cant be loaded, exit the whole process
// no recovery
scmp_filter_ctx get_default_seccomp_filter() {
  static scmp_filter_ctx ctx = seccomp_init(SCMP_ACT_TRACE(0));
  if(ctx == 0) {
    perror("seccomp_init");
    _exit(-1);
  }

  short* handlers = get_syscall_handlers();

  for(int i = 0; i < MAX_SYSCALL; i++) {
    if(handlers[i] == HANDLER_WHITELIST) {
      // add to whitelist
      if(seccomp_rule_add(ctx, SCMP_ACT_ALLOW, i, 0)) {
        perror("seccomp_rule_add");
        _exit(-1);
      }
    }
    // if not whitelisted, it probably traps
  }
  
  // special errno syscalls
  seccomp_rule_add(ctx, SCMP_ACT_ERRNO(EACCES), SCMP_SYS(socket), 0);
  seccomp_rule_add(ctx, SCMP_ACT_ERRNO(EPERM), SCMP_SYS(mkdir), 0);
  seccomp_rule_add(ctx, SCMP_ACT_ERRNO(EPERM), SCMP_SYS(unlink), 0);
  
  // fork and vfork: straight NO
  seccomp_rule_add(ctx, SCMP_ACT_ERRNO(EAGAIN), SCMP_SYS(fork), 0);
  seccomp_rule_add(ctx, SCMP_ACT_ERRNO(EAGAIN), SCMP_SYS(vfork), 0);

  return ctx;
}

scmp_filter_ctx default_seccomp_filter = get_default_seccomp_filter();