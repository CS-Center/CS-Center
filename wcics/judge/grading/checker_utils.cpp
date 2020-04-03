#include "stdio.h"

#include "checker_utils.hpp"

checker_result do_builtin_check(checker_union& cu, checker_args& ca);

checker_result do_custom_check(checker_union& cu, checker_args& ca);

checker_callable::checker_callable(const char* name) : func(do_builtin_check) {
  cu.cfunc = get_checker(name);
}

checker_callable::checker_callable(Executor* exec) : func(do_custom_check) {
  cu.exec = exec;
}

checker_result checker_callable::operator() (checker_args& ca) {
  return func(cu, ca);
}

checker_result do_builtin_check(checker_union& cu, checker_args& ca) {
  checker_sig cfunc = cu.cfunc;
  
  int r = cfunc(ca.judge_out_fd, ca.user_out_fd, ca.checker_arg);
  
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