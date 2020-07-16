#include <stdio.h>

#include "consts.hpp"
#include "utils/args.hpp"

#include "checker_utils.hpp"

checker_result builtin_checker_policy::operator () (int casenum, int in_fd, int judge_out_fd, int user_out_fd) {  
  int r = func(judge_out_fd, user_out_fd, arg);
  
  checker_result ret;
  
  if(r == 1) {
    ret.ac = true;
    ret.points = ca.points;
  }
  else if(r == 0) {
    ret.ac = false;
    ret.points = 0;
  }
  else {
    fputs("Invalid return value from builtin checker! This is an error.");
    ret.points = -1;
  }
  
  return ret;
}

builtin_checker_policy::builtin_checker_policy(const submission_info& si) : func(get_checker(si.problem.checker_id)), arg(si.problem.checker_arg) {}

builtin_checker_policy::set_suite(int suite, int pts) { points = pts; }


custom_checker_policy::custom_checker_policy() {}

custom_checker_policy::init(const submission_info& si) {
  
  if(shres.init()) {
    perror("custom_checker_policy::init: failed to initialize SharedProcessResult");
    return -1;
  }
  
  if(se.init(si.checker_lang, CODE_FILE_NAME, si.checker_code, blank_args, 0, conf, fac, shres)) {
    perror("custom_checker_policy::init: failed to initialize scoped executor");
    return -1;
  }

  

}