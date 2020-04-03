#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "absolute_precision.hpp"

#include "tokenizer.hpp"

typedef long double ld;

int absolute_token_check(const char* expected, const char* user, const char* arg) {
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
  
  if(argptr == arg)
    return -1;
    
  epsilon = powl(10, -epsilon);
  
  return abs(edub - udub) <= epsilon;
}

int absolute_precision_check(int jfd, int ufd, const char* arg) {
  return check_tokens(jfd, ufd, absolute_token_check, arg);
}