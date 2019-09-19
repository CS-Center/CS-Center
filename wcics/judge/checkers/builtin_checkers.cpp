#include <errno.h>
#include <string.h>

#include "builtin_checkers.hpp"

#include "builtin/standard.hpp"
#include "builtin/identical.hpp"
#include "builtin/standard_precision.hpp"
#include "builtin/absolute_precision.hpp"
#include "builtin/relative_precision.hpp"
#include "builtin/unordered.hpp"

int builtin_check(const char* checkername, const char* expected, const char* user, const char* arg) {
  if(strcmp(checkername, "standard") == 0)
    return standard_check(expected, user, arg);
  
  if(strcmp(checkername, "identical") == 0)
    return identical_check(expected, user, arg);
    
  if(strcmp(checkername, "standard_precision") == 0)
    return standard_precision_check(expected, user, arg);
    
  if(strcmp(checkername, "relative_precision") == 0)
    return relative_precision_check(expected, user, arg);
    
  if(strcmp(checkername, "absolute_precision") == 0)
    return absolute_precision_check(expected, user, arg);
    
  if(strcmp(checkername, "unordered") == 0)
    return unordered_check(expected, user, arg);
    
  errno = ENOENT;
  return -1;
}