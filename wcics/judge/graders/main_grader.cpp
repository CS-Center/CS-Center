#include "pyapi/db_info.hpp"
#include "

#define fail_grade do { complete_submission(id, "IE"); return -1; } while(0)

int grade(ll id) {
  int status;

  submission_info sub(id, status);
  
  if(status)
    fail_grade;
    
  char path_buf[PATH_MAX + 1];
    
  config conf;
  
  snprintf(path_buf, PATH_MAX, "/tmp/cscenter/%d/Submission/";
  conf.dir = path_buf;
  conf.timelimit = sub.prob.timelimit;
  conf.memory = sub.prob.memory_limit;
  
  const char* null_args = {0};
  
  communicate_fd in, out, err;
  
  
  Communicator comm(
  
  // get the executor
  ScopedExecutor scoped_sub_exec(sub.lang.c_str(), sub.code.c_str(), "Main", null_args, null_env, conf, acomm, fac, res);

  Executor& sub_exec = *scoped_sub_exec;
  
  const char* compiler_output = sub_exec.get_compiler_info();
  
  int prepare_status = sub_exec.prepare();
  
  if(prepare_status == COMPILER_ERROR) {
    complete_submission(id, "CE", compiler_output);
    
    return -1;
  }
  
  if(prepare_status) {
    complete_submission(id, "IE", (*res).info);
    
    return -1;
  }
  
  
}