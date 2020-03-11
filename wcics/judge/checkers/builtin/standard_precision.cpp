#include "standard_precision.hpp"

#include "tokenizer.hpp"

#include "relative_precision.hpp"
#include "absolute_precision.hpp"

// This will succeed if abs difference _or_ rel difference is within range
int standard_token_check(const char* expected, const char* user, const char* arg) {
  // If theres any result from the relative token check, return that, even if it's an error. Otherwise, compare absolute.
  int r1 = relative_token_check(expected, user, arg);
  
  if(r1 != 0)
    return r1;
    
  return absolute_token_check(expected, user, arg);
}

int standard_precision_check(const char* expected, const char* user, const char* arg) {
  return check_tokens(expected, user, standard_token_check, arg);
}