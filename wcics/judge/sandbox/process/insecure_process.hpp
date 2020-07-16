#pragma once

#include "process.hpp"

class InsecureProcess : public Process {
public:
  void monitor();
  
  void child_func();
};