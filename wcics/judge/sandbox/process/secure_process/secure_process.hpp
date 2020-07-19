#pragma once

#include <limits.h>
#include <sys/user.h>
#include <set>
#include <string>
#include <vector>
#include <thread>

#include "sandbox/process/process.hpp"
#include "secure_process_shocker.hpp"
#include "sandbox/file-access-checker/file_access_checker.hpp"

typedef unsigned long long ull;

class SecureProcess : public Process {
private:
  // the thread ID currently in use
  pid_t tid;
  
  std::set<pid_t> children;

  // the user registers
  user_regs_struct regs;
  
  // shocker
  SecureProcessShocker sps;
  std::thread shock_thread;
  
  // The file access checker
  FileAccessChecker& fac;

  // get registers
  // return status
  int get_regs();
  
  // set registers
  // return status
  int set_regs();
  
  // get syscall
  int get_syscall();
  
  // update the syscall result
  void update_result(ull);
  
  // skip the syscall
  void skip_call();
  
  // get arguments
  // only valid after get_regs
  ull get_arg1();
  ull get_arg2();
  ull get_arg3();
  
  // read file path from strptr
  int readpath(ull strptr, std::string&);
  
  // file access check
  // return -1 on error
  // return 0 on success
  // return positive number is interpreted as errno to send
  int file_access_check(int dirfd, ull strptr, int flags);
  
  // overridden virtual functions
  virtual void monitor();
  
  virtual void child_func();
  
  // kill the process and update res if they call invalid syscall
  void death_illegal(int call_no);
  
  // kill the process and fill res with a message
  // if msg is null, just kill the process
  void death_ie(const char* msg);
  
  // Kill the process and call _wait_death
  void terminate();
  
  // Wait for the process to die by signal
  // used by death_ie and death_illegal to wait for it to die by SIGKILL
  void _wait_death();
  
  // declare friends :D
  friend int empty_check(SecureProcess&);
  
  friend int exec_check(SecureProcess&);
  
  friend int stat_check(SecureProcess&);
  friend int fstatat_check(SecureProcess&);
  friend int open_check(SecureProcess&);
  friend int openat_check(SecureProcess&);
  
  friend int clone_check(SecureProcess&);
  
  friend int self_check(SecureProcess&);
  friend int prlimit_check(SecureProcess&);
    
  friend int prctl_check(SecureProcess&);
    
  friend class SecureProcessShocker;
  
public:
  SecureProcess(
    const char*,
    const char* const*,
    const char* const*,
    config&,
    SharedProcessResult&,
    FileAccessChecker&
  );
};