#pragma once

#include "process.hpp"

class InsecureProcess : public Process {
public:
  InsecureProcess(
    AsyncCommunicator& comm,
    const char* file, 
    const char* const* args, 
    const char* const* env, 
    config& conf,
    SharedProcessResult& pres
  );

  int monitor();
  
  void child_func();
};