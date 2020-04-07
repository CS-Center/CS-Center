#pragma once

#include "executors/scoped_executor.hpp"
#include "checkers/builtin_checkers.hpp"
#include "pyapi/db_info.hpp"

struct checker_result {
  bool ac;
  int points;
};

// interface for checking policies, both builtin checkers and custom checkers
struct checker_policy {  
  virtual void set_suite(int suite, int points) = 0;
  
  virtual checker_result operator() (int casenum, int in_fd, int judge_out_fd, int user_out_fd) = 0;
};

struct builtin_checker_policy : checker_policy {
  
  checker_sig func;
  const char* arg;
  int points;
  
  builtin_checker_policy(const submission_info& si);
  
  virtual void set_suite(int suite, int points);
  
  virtual checker_result operator() (int casenum, int in_fd, int judge_out_fd, int user_out_fd);
  
};

struct custom_checker_policy : checker_policy {

  ScopedExecutor se;
  SharedProcessResult shres;
  config conf;
  FileAccessChecker fac;
  
  char points_env[64];
  char suite_env[64];
  char code_file_env[64];
  
  custom_checker_policy();
  
  int init(const submission_info& si);
  
  virtual void set_suite(int suite, int points);
  
  virtual checker_result operator() (int casenum, int in_fd, int judge_out_fd, int user_out_fd);  
};