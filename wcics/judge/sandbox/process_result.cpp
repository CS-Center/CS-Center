extern const char* const sys_siglist[];

#include <string.h>
#include <sys/mman.h>
#include <errno.h>
#include <stdio.h>
#include <algorithm>

#include "process_result.hpp"
#include "syscalls/syscall_names.hpp"
#include "utils/debug.hpp"

using namespace std;

process_result::process_result() {
  reset();
}

void reset() {
  rss = -1;
  time = -1;
  
  info[0] = 0;
  info_ind = 0;
  
  // this ensures that there will at least be SOME terminator
  info[INFO_BUF_LEN] = 0;
}

void process_result::death_normal(int status) {
  death_type = DEATH_NORMAL;
  exit_info = status;
  
  snprintf(buf_ptr(), INFO_BUF_LEN - info_ind + 1, "Process died with %d\n", status);
  
  adv_ind();
}

void process_result::death_signal(int sig) {
  death_type = DEATH_SIGNAL;
  exit_info = sig;
  
  snprintf(buf_ptr(), INFO_BUF_LEN - info_ind + 1, "Process killed by signal: %s\n", sys_siglist[sig]);
  
  adv_ind();
}

void process_result::death_tle() {
  death_type = DEATH_TLE;
  
  add_msg("Process TLEd!\n");
}

void process_result::death_ill(int call_no) {
  death_type = DEATH_ILL;
  exit_info = call_no;
  
  if(syscall_names[call_no])
    snprintf(buf_ptr(), INFO_BUF_LEN - info_ind + 1, "Process invoked illegal syscall %s\n", syscall_names[call_no]);
  else
    snprintf(buf_ptr(), INFO_BUF_LEN - info_ind + 1, "Process invoked unknown syscall with number %d\n", call_no);
    
  adv_ind();
}

void process_result::death_ie(const char* msg, bool add_errno) {
  info[INFO_BUF_LEN] = 0;

  death_type = DEATH_IE;
  exit_info = errno;

  if(msg) {
    add_msg(msg);

    if(add_errno) {
      add_msg(": ");
      add_msg(strerror(errno));
      add_msg("\n");
    }
  }
}

void process_result::death_ole() {
  death_type = DEATH_OLE;
  
  add_msg("Process OLE'd!\n");
}

void process_result::death_mle() {
  death_type = DEATH_MLE;
  
  add_msg("Process MLE'd!\n");
}



void process_result::add_msg(const char* s) {
  int l = strlen(s);

  strncpy(info + info_ind, s, min(INFO_BUF_LEN - info_ind, l));
  
  info_ind += l;
}

char* process_result::buf_ptr() {
  return info + info_ind;
}

void process_result::adv_ind() {
  info_ind += strlen(info + info_ind);
}



SharedProcessResult::SharedProcessResult() {
  ptr = (process_result*)
    RUNTIME_FUNC_EQ(mmap(0, sizeof(process_result), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0), MAP_FAILED);
  
  // call constructor
  new (ptr) process_result;
}

SharedProcessResult::~SharedProcessResult() {
  RUNTIME_FUNC(munmap(ptr, sizeof(process_result)));
}

process_result& SharedProcessResult::operator* () {
  return *ptr;
}