#pragma once

#include "utils/async_launcher.hpp"

class SecureProcess;

class SecureProcessShocker {
  SecureProcess& proc;
  
public:
  SecureProcessShocker(SecureProcess&);
  
  int launch();
};

using AsyncShocker = AsyncLauncher<SecureProcessShocker>;