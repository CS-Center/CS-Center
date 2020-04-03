#include "executors/executor.hpp"
#include "checkers/builtin_checkers.hpp"

union checker_union {
  checker_sig cfunc;
  Executor* exec;
};

struct checker_result {
  bool ac;
  int points;
};

// all the arguments passed to the checker
// custom checkers can use all of these, but builtin checkers use barely any
struct checker_args {
  const char* checker_arg;

  int points, output_limit, case_num, suite_num;
    
  int user_out_fd;
  
  int judge_out_fd;
};

typedef checker_result (*_do_check)(checker_union&, checker_args&);

struct checker_callable {
  checker_union cu;
  _do_check func;
  
  checker_callable(Executor* checker_exec);
  checker_callable(const char* name);
  
  checker_result operator() (checker_args& ca);
};