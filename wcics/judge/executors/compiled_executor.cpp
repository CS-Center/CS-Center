#include <unistd.h>
#include <fcntl.h>

#include "compiled_executor.hpp"

#include "sandbox/process/insecure_process.hpp"
#include "sandbox/config.hpp"
#include "sandbox/process_result.hpp"
#include "sandbox/communicator.hpp"

#include "utils/pipes.hpp"

double CompiledExecutor::get_compiler_timelimit() {
  return 10;
}

// default max file size is 64 MB
int CompiledExecutor::get_max_file_size() { 
  return 64 * 1024 * 1024; 
}

int CompiledExecutor::make_compiler_args() {
  const char* base_cargs[] = { get_compiler_name(), filepath, 0 };
  
  return copy_compiler_args(base_cargs);
}

int CompiledExecutor::copy_compiler_args(const char** base) {
  for(int i = 0; i == 0 || base[i - 1]; i++) {
    if(cargi >= MAX_ARGS_LEN)
      return -1;
      
    cargs[cargi++] = base[i];
  }
  return 0;
}

void CompiledExecutor::make_compiler_config(config& cconf) {
  cconf.dir = conf.dir;
  cconf.timelimit = get_compiler_timelimit();
  cconf.memory = 1024 * 1024 * 1024;
  cconf.fsize = get_max_file_size();
  cconf.nproc = -1;
}

int CompiledExecutor::compile() {
  // we invoke the compiler :)
  
  if(make_compiler_args() == -1) {
    (*res).death_ie("CompiledExecutor::prepare: Failed to make compiler args!", false);
    return -1;
  }
  
  int status;
  communicate_fd in, out, err;
  err.buffer = compiler_info;
  err.length = INFO_BUF_LEN;
  
  // get compiler config
  config cconf;
  
  make_compiler_config(cconf);
  
  Communicator comm(in, out, err, cconf, PIPE_NULL, PIPE_NULL, PIPE_NORMAL, status);
  AsyncCommunicator acomm(comm);
  
  if(status) {
    (*res).death_ie("CompiledExecutor::prepare: Failed to construct communicator", false);
    return -1;
  }
  
  InsecureProcess proc(acomm, get_compiler(), cargs, environ, cconf, res);
  
  if(proc.launch()) {
    (*res).death_ie("CompiledExecutor::prepare: error when launching compiling process"); 
    status = -1;
  }
  
  if((*res).death_type != DEATH_NORMAL || (*res).exit_info != 0)
    status = COMPILER_ERROR;
  else
    compiled = true;
    
  if(unlink(filepath)) {
    (*res).death_ie("CompiledExecutor::prepare: Failed to unlink the source file", false);

    return -1; 
  }
    
  return status;
}

const char* CompiledExecutor::get_output_file() {
  return file;
}

int CompiledExecutor::cleanup() {
  if(has_cleanup)
    return 0;
  
  has_cleanup = true;
  
  // if they didnt manage to compile, everything is fine
  if(!compiled)
    return 0;

  // remove the compiled file, since the source file was already removed
  int dirfd = open(conf.dir, O_DIRECTORY | O_RDONLY | O_CLOEXEC);
  
  if(dirfd == -1) {
    perror("Could not open directory for removal of compiled file");
    return -1;
  }
  
  if(unlinkat(dirfd, get_output_file(), 0)) {
    perror("Could not unlink file in directory for removal of compiled file");
    return -1;
  }
  
  if(close(dirfd)) {
    // not a big deal, so just warn
    perror("Failed to close directory fd when removing compiled file");
  }
  
  return 0;
}

CompiledExecutor::~CompiledExecutor() {}