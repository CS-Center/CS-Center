#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include "compiled_executor.hpp"

#include "sandbox/process/insecure_process.hpp"
#include "sandbox/config.hpp"
#include "sandbox/process_result.hpp"

#include "utils/pipes.hpp"
#include "utils/files.hpp"
#include "utils/debug.hpp"

double CompiledExecutor::get_compiler_timelimit() {
  return 10;
}

// default max file size is 64 MB
int CompiledExecutor::get_max_file_size() { 
  return 64 * 1024 * 1024; 
}

void CompiledExecutor::make_compiler_args() {
  cargs = { get_compiler_name() };
  
  for(const char* c : get_compiler_flags())
    cargs.push_back(c);
    
  cargs.push_back(source_filename.c_str());
  cargs.push_back(0);
}

std::vector<const char*> CompiledExecutor::get_compiler_flags() { return {}; }

void CompiledExecutor::make_compiler_config(config& cconf) {
  cconf.dir = conf.dir;
  cconf.timelimit = get_compiler_timelimit();
  cconf.memory = 1024 * 1024 * 1024;
  cconf.fsize = get_max_file_size();
  cconf.nproc = -1;
}

process_result* CompiledExecutor::get_compiler_result() {
  return &(*comp_res);
}

std::string CompiledExecutor::get_compiler_output() {
  return compiler_output;
}

void CompiledExecutor::compile() {
  // we invoke the compiler :)
  
  make_compiler_args();
  
  // get compiler config
  config cconf;
    
  make_compiler_config(cconf);
      
  int temp_fd = RUNTIME_FUNC(open("/tmp", O_RDWR | O_TMPFILE, 0600));
  
  std::vector<const char*> null_env = {0};
  
  cconf.set_streams(null_read_fd, null_write_fd, temp_fd);
  
  InsecureProcess proc(get_compiler_exec(), cargs.data(), null_env.data(), cconf, comp_res);
    
  proc.launch();
  
  compiled = true;
  
  compiler_output = read_from_file(temp_fd);
  
  compiled_filename = std::string(base_filename) + get_compiled_ext();
  compiled_filepath = conf.dir + compiled_filename;
  
  RUNTIME_FUNC(close(temp_fd));
}

const char* CompiledExecutor::get_compiled_ext() {
  return "";
}

void CompiledExecutor::cleanup() {
  if(has_cleanup)
    return;
  
  has_cleanup = true;
    
  // remove the source file
  RUNTIME_FUNC(unlink(filepath.c_str()));
  
  // if they didnt manage to compile, everything is fine
  if(!compiled)
    return;

  RUNTIME_FUNC(unlink(compiled_filename.c_str()));
}

CompiledExecutor::~CompiledExecutor() {}