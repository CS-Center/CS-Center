#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <algorithm>

#include "relative_precision.hpp"

#include "tokenizer.hpp"

typedef long double ld;

int relative_token_check(const char* expected, const char* user, const char* arg) {
  char* eptr, *uptr, *argptr;
  
  ld edub = strtold(expected, &eptr);
  ld udub = strtold(user, &uptr);
  
  bool b1 = uptr != user + strlen(user);
  bool b2 = eptr != expected + strlen(expected);
  
  // conversion for both failed, so compare the raw tokens
  if(b1 && b2)
    return strcmp(expected, user) == 0;
    
  // one of the two tokens failed, so they are not equal enough
  if(b1 || b2)
    return 0;
    
  ld epsilon = strtold(arg, &argptr);
  
  if(argptr != arg + strlen(arg)) {
    errno = EINVAL;
    return -1;
  }
    
  epsilon = powl(10, -epsilon);
  
  ld r1 = (1 - epsilon) * edub;
  ld r2 = (1 + epsilon) * edub;
  
  return std::min(r1, r2) <= udub && udub <= std::max(r1, r2);
}

int relative_precision_check(const char* expected, const char* user, const char* arg) {
  return check_tokens(expected, user, relative_token_check, arg);
}