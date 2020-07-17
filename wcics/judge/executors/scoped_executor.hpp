#include <string>
#include <vector>

#include "executor_info.hpp"

class ScopedExecutor {
public:
  Executor* ptr;

  ScopedExecutor(const char* id, std::string code, const char* file, std::vector<const char*> extra_args, const char* const* env, config& conf, FileAccessChecker& fac, SharedProcessResult& res);
  
  Executor& operator*();
  
  ~ScopedExecutor();
};