#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <utility>

#include "executor.hpp"
#include "utils/files.hpp"
#include "utils/debug.hpp"

using namespace std;

int Executor::get_nproc() {
  return 0;
}

const char* Executor::get_exec() {
  exec = string("./") + base_filename;

  return exec.c_str();
}

const char* Executor::get_exec_name() {
  // by default, same as get_exec
  return get_exec();
}

void Executor::make_args() {
  args = {get_exec_name()};
  
  for(const char* s : extra_args)
    args.push_back(s);
    
  args.push_back(0);
}

Executor::Executor(string code, const char* file, vector<const char*> extra_args, const char* const* env, config& conf,  FileAccessChecker& fac, SharedProcessResult& res) :
  base_filename(file),
  code(code),
  extra_args(extra_args),
  env(env),
  conf(conf),
  fac(fac),
  res(res)
{}

const char* Executor::get_source_ext() { return ""; }

void Executor::create_files() {
  // this isnt really a create_files step, but its necessary
  conf.nproc = get_nproc();
  
  source_filename = string(base_filename) + get_source_ext();
  
  filepath = string(conf.dir) + "/" + source_filename;
  
  write_to_file(filepath.c_str(), code);
  
  make_args();
}

void Executor::compile() {}

process_result* Executor::get_compiler_result() {
  return 0;
}

std::string Executor::get_compiler_output() { return ""; }

void Executor::prepare() {
  create_files();
    
  compile();
}

void Executor::cleanup() {
  if(has_cleanup)
    return;
    
  has_cleanup = true;
  
  RUNTIME_FUNC(unlink(filepath.c_str()));
}

void Executor::launch() {
  SecureProcess proc(get_exec(), args.data(), env, conf, res, fac);
  
  proc.launch();
}