#include <ctype.h>

#include "standard.hpp"

int standard_check(const char* expected, const char* user, const char* arg) {
  // ideally this checker is very lenient, and should be the default
  // it considers all whitespace equally
  // it doesn't care about trailing whitespace
  // basically it searches for the next non-whitespace character
  // and if any are not the same, fail
  
  int ei = 0, ui = 0;
  
  while(1) {
  
    bool s1white = false, s2white = false;
    
    // search for the next space
    while(expected[ei] && isspace(expected[ei])) {
      s1white = true;
      ei++;
    }
      
    while(user[ui] && isspace(user[ui])) {
      s2white = true;
      ui++;
    }
      
    // there must either be no whitespace or both whitespace
    if(s1white != s2white)
      return 0;
    
    if(user[ui] != expected[ei])
      return 0;
      
    // this is valid because if one is null and the other isnt, we will exit as above. If they are both null, we get here and exit
      // if neither is null, we continue
    if(user[ui] == 0)
      return 1;
      
    ei++;
    ui++;
  }
}