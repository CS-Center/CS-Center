#include <sys/syscall.h>

#include "syscalls.hpp"
#include "inspectors.hpp"

inspector_sig* init_inspectors() {
  // define the list of syscall inspectors for syscalls who have them
  // it is an error to inspect a syscall without an inspector
  static inspector_sig arr[MAX_SYSCALL];
  
  // fill the array with empty handlers
  for(int i = 0; i < MAX_SYSCALL; i++)
    arr[i] = empty_check;
    
  // file access inspectors
  arr[SYS_open] = open_check;
  arr[SYS_lstat] = stat_check;
  arr[SYS_stat] = stat_check;
  arr[SYS_statfs] = stat_check;
  arr[SYS_access] = stat_check;
  arr[SYS_readlink] = stat_check;
  arr[SYS_openat] = openat_check;
  arr[SYS_newfstatat] = fstatat_check;
  arr[SYS_readlinkat] = stat_check;
  arr[SYS_faccessat] = fstatat_check;
  
  // execve inspector
  arr[SYS_execve] = exec_check;
  
  // kill inspector
  arr[SYS_kill] = self_check;
  
  // clone
  arr[SYS_clone] = clone_check;
  
  // prctl
  arr[SYS_prctl] = prctl_check;
  
  // prlimit check
  arr[SYS_prlimit64] = prlimit_check;
  
  return arr;
}

inspector_sig* syscall_inspectors = init_inspectors();