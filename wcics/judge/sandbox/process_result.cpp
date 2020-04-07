// this is required to get the posix compliant version of strerror_r
// which is much better
#undef _GNU_SOURCE
#include <string.h>

extern const char* const sys_siglist[];

#include <new>
#include <sys/mman.h>
#include <errno.h>
#include <stdio.h>

#include "process_result.hpp"
#include "syscalls/syscall_names.hpp"

process_result::process_result() {
  info[0] = 0;
}

void process_result::death_normal(int status) {
  death_type = DEATH_NORMAL;
  exit_info = status;
  
  snprintf(info, INFO_BUF_LEN, "Process died with %d", status);
}

void process_result::death_signal(int sig) {
  death_type = DEATH_SIGNAL;
  exit_info = sig;
  
  snprintf(info, INFO_BUF_LEN, "Process killed by signal: %s", sys_siglist[sig]);
}

void process_result::death_tle() {
  death_type = DEATH_TLE;
  
  strncpy(info, "Process TLEd!", INFO_BUF_LEN);
}

void process_result::death_ill(int call_no) {
  death_type = DEATH_ILL;
  exit_info = call_no;
  
  if(syscall_names[call_no])
    snprintf(info, INFO_BUF_LEN, "Process invoked illegal syscall %s", syscall_names[call_no]);
  else
    snprintf(info, INFO_BUF_LEN, "Process invoked unknown syscall with number %d", call_no);
}

void process_result::death_ie(const char* msg, bool add_errno) {
  info[INFO_BUF_LEN] = 0;

  death_type = DEATH_IE;
  exit_info = errno;

  if(msg) {
    int msglen = strlen(msg);
    strncpy(info, msg, INFO_BUF_LEN);

    if(add_errno) {
      strncat(info, ": ", INFO_BUF_LEN - msglen);
      // This can throw ERANGE, but since this is already an error handler, we consider this a "best attempt"
      strerror_r(errno, info + msglen + 2, INFO_BUF_LEN - msglen - 2);
    }
  }
}

void process_result::death_ole() {
  death_type = DEATH_OLE;
  
  strncpy(info, "Process OLEd!", INFO_BUF_LEN);
}

void process_result::death_mle() {
  death_type = DEATH_MLE;
  
  strncpy(info, "Process MLEd!", INFO_BUF_LEN);
}

SharedProcessResult::SharedProcessResult() : ptr(0) {}

int SharedProcessResult::init() {
  ptr = (process_result*)mmap(0, sizeof(process_result), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  
  if(ptr == 0) {
    perror("Failed to mmap for SharedProcessResult");
    return -1;
  }
  
  // call constructor
  new (ptr) process_result;
  
  return 0;
}

SharedProcessResult::~SharedProcessResult() {
  // if munmap fails, we probably have a memory leak
  // However, we cant throw an exception or anything...
  // the best we can do is print an error
  if(munmap(ptr, sizeof(process_result)))
    perror("SharedProcessResult: munmap");
}

process_result& SharedProcessResult::operator* () {
  return *ptr;
}