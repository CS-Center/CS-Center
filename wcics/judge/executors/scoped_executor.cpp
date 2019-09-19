#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "scoped_executor.hpp"

ScopedExecutor::ScopedExecutor(const char* id, const char* code, const char* file, const char* const* extra_args, const char* const* env, config& conf, AsyncCommunicator& acomm, FileAccessChecker& fac, SharedProcessResult& res) {
  for(const ExecutorInfo& ei : executors)
    if(strcmp(ei.shortname, id) == 0)
      return ei.make_executor(code, file, extra_args, env, conf, acomm, fac, res);
      
  
  ptr = 0;
  errno = ENOENT;
  fprintf(stderr, "Could not retrieve executor for unknown language id '%s'\n");
}

Executor& ScopedExecutor::operator*() {
  if(ptr)
    return *ptr;
    
  fputs("Fatal error: dereference of null pointer in ScopedExecutor\n", stderr);
  _exit(-1);
}

ScopedExecutor::~ScopedExecutor() {
  // free the pointer
  delete ptr;
}