#pragma once

#include "utils/async_launcher.hpp"

class SecureProcess;

class SecureProcessShocker {
  SecureProcess& proc;
  
public:
  SecureProcessShocker(SecureProcess&);
  
  void launch();
};
