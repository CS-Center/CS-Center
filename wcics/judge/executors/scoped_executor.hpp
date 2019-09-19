#include "executor_info.hpp"

class ScopedExecutor {
public:
  Executor* ptr;

  ScopedExecutor(const char* name);
  
  Executor& operator*();
  
  ~ScopedExecutor();
};