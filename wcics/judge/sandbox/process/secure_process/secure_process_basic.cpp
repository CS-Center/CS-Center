#include <sys/ptrace.h>

#include "secure_process.hpp"

int SecureProcess::get_regs() {
  return ptrace(PTRACE_GETREGS, tid, 0, &regs);
}

int SecureProcess::set_regs() {
  return ptrace(PTRACE_SETREGS, tid, 0, &regs);
}

int SecureProcess::get_syscall() {
  return regs.orig_rax;
}

void SecureProcess::update_result(ull val) {
  regs.rax = val;
}

void SecureProcess::skip_call() {
  regs.orig_rax = -1;
}

// no more args are necessary

ull SecureProcess::get_arg1() {
  return regs.rdi;
}

ull SecureProcess::get_arg2() {
  return regs.rsi;
}

ull SecureProcess::get_arg3() {
  return regs.rdx;
}

SecureProcess::SecureProcess(
  const char* file, 
  const char* const* args, 
  const char* const* env, 
  config& conf,
  SharedProcessResult& pres,
  FileAccessChecker& checker
) :
  Process(file, args, env, conf, pres), 
  sps(*this),
  shock(sps),
  fac(checker)
{}

void SecureProcess::death_illegal(int call_no) {
  res.death_ill(call_no);
  
  terminate();
  wait_death();
}

void SecureProcess::death_ie(const char* msg) {
  res.death_ie(msg);
  
  terminate();
  wait_death();
}