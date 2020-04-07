#pragma once

#define NO_DEATH 0
#define DEATH_NORMAL 1
#define DEATH_SIGNAL 2
#define DEATH_TLE 3
#define DEATH_ILL 4
#define DEATH_IE 5
#define DEATH_OLE 6
#define DEATH_MLE 7

#define INFO_BUF_LEN 256
typedef char info_buf[INFO_BUF_LEN + 1];

// a structure to contain results for a process
// contains things like exit code, (or killing signal)
// and other codes depending on TLE, ILL

struct process_result {
  long rss = -1;
  
  // time spent
  double time;

  // one of the above constants, the death type of the process
  int death_type = NO_DEATH;
  
  // the extra info (exit status, signal, 0, ill syscall, errno?, 0)
  int exit_info;
  
  // info string
  info_buf info;
  
  void death_normal(int status);
  
  void death_signal(int sig);
  
  void death_tle();
  
  void death_ill(int call_no);
  
  void death_mle();
  
  // make an error message out of (possibly) errno and str
  void death_ie(const char* str, bool add_errno = true);
  
  void death_ole();
  
  process_result();
};

// a class to facilitate sharing of process_result for initialization errors
class SharedProcessResult {
  process_result* ptr;

public:
  // make a null SPR
  SharedProcessResult();
  
  int init();
  
  process_result& operator*();
  
  ~SharedProcessResult();
};