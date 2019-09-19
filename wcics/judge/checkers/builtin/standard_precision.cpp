#include "standard_precision.hpp"

#include "tokenizer.hpp"

#include "relative_precision.hpp"
#include "absolute_precision.hpp"

int standard_token_check(const char* expected, const char* user, const char* arg) {
  int r1 = relative_token_check(expected, user, arg);
  
  if(r1 != 0)
    return r1;
    
  return absolute_token_check(expected, user, arg);
}

int standard_precision_check(const char* expected, const char* user, const char* arg) {
  return check_tokens(expected, user, standard_token_check, arg);
}