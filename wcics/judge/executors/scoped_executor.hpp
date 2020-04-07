#include "executor_info.hpp"

class ScopedExecutor {
public:
  Executor* ptr;

  ScopedExecutor();
  
  int init(const char* id, const char* code, const char* file, const char* const* extra_args, const char* const* env, config& conf, FileAccessChecker& fac, SharedProcessResult& res);
  
  Executor& operator*();
  
  ~ScopedExecutor();
};