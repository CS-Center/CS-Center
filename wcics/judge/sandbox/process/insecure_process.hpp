#pragma once

#include "process.hpp"

class InsecureProcess : public Process {
public:
  using Process::Process;

  void monitor();
  
  void child_func();
};