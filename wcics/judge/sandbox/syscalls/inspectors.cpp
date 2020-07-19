#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/prctl.h>
#include <sys/syscall.h>

#include "inspectors.hpp"

int exec_check (SecureProcess& proc) {
  static bool has_execve = false;
  
  if(has_execve) {
    proc.res.death_ill(SYS_execve);
    proc.res.death_type = DEATH_ILL;
    
    return -1;
  }
  
  has_execve = true;
  return 0;
}

int empty_check (SecureProcess& proc) {
  proc.res.death_ie("Error: syscall without inspector called!\n", false);
  return -1;
}

int stat_check (SecureProcess& proc) {
  ull strptr = proc.get_arg1();
  return proc.file_access_check(AT_FDCWD, strptr, 0);
}

int fstatat_check (SecureProcess& proc) {
  int dirfd = proc.get_arg1();
  ull strptr = proc.get_arg2();
  return proc.file_access_check(dirfd, strptr, 0);
}

int open_check (SecureProcess& proc) {
  ull strptr = proc.get_arg1();
  int flags = proc.get_arg2();
  return proc.file_access_check(AT_FDCWD, strptr, flags);
}

int openat_check (SecureProcess& proc) {
  int dirfd = proc.get_arg1();
  ull strptr = proc.get_arg2();
  int flags = proc.get_arg3();
  return proc.file_access_check(dirfd, strptr, flags);
}

int clone_check (SecureProcess& proc) {
  ull flags = proc.get_arg1();
  if(flags & CLONE_THREAD)
    return 0;
  return EAGAIN;
}

// check that arg1 is the pid
int self_check(SecureProcess& proc) {
  pid_t p = proc.get_arg1();
  if(p == proc.pid)
    return 0;
  return EPERM;
}

int prlimit_check(SecureProcess& proc) {
  pid_t p = proc.get_arg1();
  if(p == 0 || p == proc.pid)
    return 0;
  return EPERM;
}

int prctl_check(SecureProcess& proc) {
  int opt = proc.get_arg1();
  if(opt != PR_GET_NAME && opt != PR_SET_NAME) {
    proc.death_illegal(SYS_prctl);
    strncpy(proc.res.info, "prctl called with invalid option", INFO_BUF_LEN);
  }
  return 0;
   
}