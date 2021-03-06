#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "relative_precision.hpp"

#include "tokenizer.hpp"

typedef long double ld;

int relative_token_check(const char* expected, const char* user, const char* arg) {
  char* eptr, *uptr, *argptr;
  
  ld edub = strtold(expected, &eptr);
  ld udub = strtold(user, &uptr);
  
  // conversion for both failed, so compare the raw tokens
  if(uptr != user + strlen(user) && eptr != expected + strlen(expected))
    return strcmp(expected, user) == 0;
    
  // one of the two tokens failed, so they are not equal enough
  if(uptr != user + strlen(user) || eptr != expected + strlen(expected))
    return 0;
    
  ld epsilon = strtold(arg, &argptr);
  
  if(argptr != arg + strlen(arg)) {
    errno = EINVAL;
    return -1;
  }
    
  epsilon = powl(10, -epsilon);
  
  return (1 - epsilon) * edub <= udub && udub <= (1 + epsilon) * edub;
}

int relative_precision_check(const char* expected, const char* user, const char* arg) {
  return check_tokens(expected, user, relative_token_check, arg);
}