#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <utility>

#include "executor.hpp"

int Executor::get_nproc() {
  return 0;
}

const char* Executor::get_exec() {
  // by default, use the file
  strncpy(exec, "./", FILE_MAX_LEN);
  strncat(exec, file, FILE_MAX_LEN - 2);

  return exec;
}

const char* Executor::get_exec_name() {
  // by default, same as get_exec
  return get_exec();
}

int Executor::make_args() {
  const char* base_args[] = { get_exec_name(), 0 };
  return add_args(base_args);
}

int Executor::add_args(const char** base_args) {
  for(int i = 0; base_args[i]; i++) {
    if(argi >= MAX_ARGS_LEN)
      return -1;
    
    args[argi++] = base_args[i];
  }

  // add all args, including null terminator
  for(int i = 0; i == 0 || extra_args[i - 1]; i++) {
    if(argi >= MAX_ARGS_LEN)
      return -1;

    args[argi++] = extra_args[i];
  }
  
  return 0;
}

Executor::Executor(const char* code, const char* file, const char* const* extra_args, const char* const* env, config& conf,  FileAccessChecker& fac, SharedProcessResult& res) :
  file(file),
  code(code),
  extra_args(extra_args),
  env(env),
  conf(conf),
  fac(fac),
  res(res)
{
  memset(exec, 0, sizeof(exec));
}

int Executor::create_files() {
  // this isnt really a create_files step, but its necessary
  conf.nproc = get_nproc();

  memset(filepath, 0, sizeof filepath);
  
  int dir_len = strlen(conf.dir);
  int file_len = strlen(file);
  
  strncpy(filepath, conf.dir, PATH_MAX);
  strncat(filepath, "/", PATH_MAX - dir_len);
  strncat(filepath, file, PATH_MAX - dir_len - 1);
  strncat(filepath, ".", PATH_MAX - dir_len - 1 - file_len);
  strncat(filepath, get_ext(), PATH_MAX - dir_len - 1 - file_len - 1);
  
  // open and create the new file
  scoped_fd code_fd("executor_code");
    
  if(code_fd.open(filepath, O_WRONLY | O_CREAT | O_EXCL, 0600)) {
    perror("Failed to open the file for writing");
    return -1;
  }
  
  if(code_fd.write(code, strlen(code)) == -1) {
    perror("Failed to write contents to file");
    return -1;
  }
  
  if(make_args() == -1) {
    fputs("Failed to make arguments\n", stderr);
    return -1;
  }
  
  return 0;
}

int Executor::compile() {
  return 0;
}

const char* Executor::get_compiler_info() {
  return compiler_info;
}

int Executor::prepare() {
  if(create_files())
    return -1;
    
  if(compile())
    return -1;
    
  return 0;
}

int Executor::cleanup() {
  if(has_cleanup)
    return 0;
    
  has_cleanup = true;
  
  if(unlink(filepath)) {
    perror("Failed to unlink the source file");
    return -1;
  }
  
  return 0;
}

int Executor::launch(file_config& file_conf) {
  SecureProcess proc(get_exec(), args, env, conf, res, fac);
  
  if(proc.launch(file_conf) == -1)
    return -1;
  
  return 0;
}