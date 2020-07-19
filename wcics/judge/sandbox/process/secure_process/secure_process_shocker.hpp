#pragma once

class SecureProcess;

class SecureProcessShocker {
  SecureProcess& proc;
  
public:
  SecureProcessShocker(SecureProcess&);
  
  void launch();
};
