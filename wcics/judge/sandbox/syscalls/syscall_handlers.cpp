#include <sys/syscall.h>

#include "syscalls.hpp"

static bool handler_init = false;

short* get_syscall_handlers() {
  // define a static variable to be initialized at runtime, before main
  static short arr[MAX_SYSCALL];

  if(handler_init) return arr;

  handler_init = true;
  
  // Read syscalls
	arr[SYS_read] = HANDLER_WHITELIST;
	arr[SYS_pread64] = HANDLER_WHITELIST;
	arr[SYS_readv] = HANDLER_WHITELIST;
	arr[SYS_preadv] = HANDLER_WHITELIST;
  
  // Write syscalls
	arr[SYS_write] = HANDLER_WHITELIST;
	arr[SYS_pwrite64] = HANDLER_WHITELIST;
	arr[SYS_writev] = HANDLER_WHITELIST;
	arr[SYS_pwritev] = HANDLER_WHITELIST;
  
  // File access syscalls
  arr[SYS_open] = HANDLER_INSPECT;
	arr[SYS_lstat] = HANDLER_INSPECT;
	arr[SYS_stat] = HANDLER_INSPECT;
  arr[SYS_statfs] = HANDLER_INSPECT;
	arr[SYS_access] = HANDLER_INSPECT;
	arr[SYS_readlink] = HANDLER_INSPECT;
	arr[SYS_openat] = HANDLER_INSPECT;
	arr[SYS_newfstatat] = HANDLER_INSPECT;
	arr[SYS_readlinkat] = HANDLER_INSPECT;
	arr[SYS_faccessat] = HANDLER_INSPECT;
  
  // File manipulation on descriptors
	arr[SYS_close] = HANDLER_WHITELIST;
	arr[SYS_fstat] = HANDLER_WHITELIST;
  arr[SYS_fstatfs] = HANDLER_WHITELIST;
	arr[SYS_poll] = HANDLER_WHITELIST;
	arr[SYS_lseek] = HANDLER_WHITELIST;
	arr[SYS_select] = HANDLER_WHITELIST;
	arr[SYS_dup] = HANDLER_WHITELIST;
	arr[SYS_dup2] = HANDLER_WHITELIST;
	arr[SYS_sendfile] = HANDLER_WHITELIST;
	arr[SYS_fcntl] = HANDLER_WHITELIST;
	arr[SYS_flock] = HANDLER_WHITELIST;
	arr[SYS_fsync] = HANDLER_WHITELIST;
	arr[SYS_fdatasync] = HANDLER_WHITELIST;
	arr[SYS_getdents] = HANDLER_WHITELIST;
	arr[SYS_readahead] = HANDLER_WHITELIST;
	arr[SYS_getdents64] = HANDLER_WHITELIST;
	arr[SYS_fadvise64] = HANDLER_WHITELIST;
	arr[SYS_pselect6] = HANDLER_WHITELIST;
	arr[SYS_ppoll] = HANDLER_WHITELIST;
	arr[SYS_sync_file_range] = HANDLER_WHITELIST;
	arr[SYS_dup3] = HANDLER_WHITELIST;
  arr[SYS_ioctl] = HANDLER_WHITELIST;

  // EPERM for mkdir and unlink
  arr[SYS_mkdir] = HANDLER_ERRNO;
  arr[SYS_unlink] = HANDLER_ERRNO;

  // Arch_prctl
  arr[SYS_arch_prctl] = HANDLER_WHITELIST;
    
  // Memory management
	arr[SYS_mmap] = HANDLER_WHITELIST;
	arr[SYS_mprotect] = HANDLER_WHITELIST;
	arr[SYS_munmap] = HANDLER_WHITELIST;
	arr[SYS_brk] = HANDLER_WHITELIST;
	arr[SYS_mremap] = HANDLER_WHITELIST;
	arr[SYS_msync] = HANDLER_WHITELIST;
	arr[SYS_mincore] = HANDLER_WHITELIST;
	arr[SYS_madvise] = HANDLER_WHITELIST;

  // Signals
	arr[SYS_rt_sigaction] = HANDLER_WHITELIST;
	arr[SYS_rt_sigprocmask] = HANDLER_WHITELIST;
	arr[SYS_rt_sigreturn] = HANDLER_WHITELIST;
	arr[SYS_pause] = HANDLER_WHITELIST;
	arr[SYS_rt_sigpending] = HANDLER_WHITELIST;
	arr[SYS_rt_sigtimedwait] = HANDLER_WHITELIST;
	arr[SYS_rt_sigsuspend] = HANDLER_WHITELIST;
	arr[SYS_sigaltstack] = HANDLER_WHITELIST;

  // Get time
	arr[SYS_gettimeofday] = HANDLER_WHITELIST;
	arr[SYS_times] = HANDLER_WHITELIST;
	arr[SYS_time] = HANDLER_WHITELIST;
	arr[SYS_clock_gettime] = HANDLER_WHITELIST;
	arr[SYS_clock_getres] = HANDLER_WHITELIST;

  // CPU scheduling
	arr[SYS_sched_yield] = HANDLER_WHITELIST;
	arr[SYS_getpriority] = HANDLER_WHITELIST;
	arr[SYS_sched_getparam] = HANDLER_WHITELIST;
	arr[SYS_sched_getscheduler] = HANDLER_WHITELIST;
	arr[SYS_sched_get_priority_max] = HANDLER_WHITELIST;
	arr[SYS_sched_get_priority_min] = HANDLER_WHITELIST;
	arr[SYS_sched_rr_get_interval] = HANDLER_WHITELIST;
	arr[SYS_sched_getaffinity] = HANDLER_WHITELIST;

  // Sleep
	arr[SYS_nanosleep] = HANDLER_WHITELIST;
	arr[SYS_clock_nanosleep] = HANDLER_WHITELIST;

  // Alarm
	arr[SYS_alarm] = HANDLER_WHITELIST;

  // kill
  arr[SYS_kill] = HANDLER_INSPECT;

  // Misc process info
	arr[SYS_getpid] = HANDLER_WHITELIST;
	arr[SYS_uname] = HANDLER_WHITELIST;
	arr[SYS_getcwd] = HANDLER_WHITELIST;
	arr[SYS_getgid] = HANDLER_WHITELIST;
  arr[SYS_getuid] = HANDLER_WHITELIST;
	arr[SYS_geteuid] = HANDLER_WHITELIST;
	arr[SYS_getegid] = HANDLER_WHITELIST;
	arr[SYS_getppid] = HANDLER_WHITELIST;
	arr[SYS_getpgrp] = HANDLER_WHITELIST;
	arr[SYS_getgroups] = HANDLER_WHITELIST;
	arr[SYS_getresuid] = HANDLER_WHITELIST;
	arr[SYS_getresgid] = HANDLER_WHITELIST;
	arr[SYS_getpgid] = HANDLER_WHITELIST;
	arr[SYS_getsid] = HANDLER_WHITELIST;
	arr[SYS_capget] = HANDLER_WHITELIST;
	arr[SYS_gettid] = HANDLER_WHITELIST;

  // rlimits
	arr[SYS_getrlimit] = HANDLER_WHITELIST;
	arr[SYS_getrusage] = HANDLER_WHITELIST;
  arr[SYS_setrlimit] = HANDLER_WHITELIST;
  
  // pipes (allowed because self-pipe hack)
  arr[SYS_pipe] = HANDLER_WHITELIST;
  arr[SYS_pipe2] = HANDLER_WHITELIST;

  // prlimit64
  arr[SYS_prlimit64] = HANDLER_INSPECT;
  
  // no fork or vfork
  arr[SYS_fork] = arr[SYS_vfork] = HANDLER_ERRNO;
  
  // allow _threads_
  arr[SYS_clone] = HANDLER_INSPECT;
  
  // prctl some things
  arr[SYS_prctl] = HANDLER_INSPECT;
  
  // EACCES sockets
  arr[SYS_socket] = HANDLER_ERRNO;
  
  // execve
  arr[SYS_execve] = HANDLER_INSPECT;
  
  // exit
  arr[SYS_exit] = HANDLER_ALLOW;
  arr[SYS_exit_group] = HANDLER_ALLOW;
  
  // system info
  arr[SYS_sysinfo] = HANDLER_WHITELIST;
  
  // Thread sync
	arr[SYS_futex] = HANDLER_WHITELIST;
	arr[SYS_set_tid_address] = HANDLER_WHITELIST;
  arr[SYS_set_robust_list] = HANDLER_WHITELIST;
  arr[SYS_get_robust_list] = HANDLER_WHITELIST;

  // Internals
	arr[SYS_restart_syscall] = HANDLER_WHITELIST;

  // Thread area
	arr[SYS_set_thread_area] = HANDLER_WHITELIST;
	arr[SYS_get_thread_area] = HANDLER_WHITELIST;
  
  return arr;
}

short* syscall_handlers = get_syscall_handlers();