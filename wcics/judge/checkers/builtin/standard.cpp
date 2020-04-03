#include <ctype.h>

#include "checkers/utils/buffers.hpp"

#include "standard.hpp"

int standard_check(int jfd, int ufd, const char* arg) {
  // ideally this checker is very lenient, and should be the default
  // it considers all whitespace equally
  // it doesn't care about trailing whitespace
  // basically it searches for the next non-whitespace character
  // and if any are not the same, fail
  
  buffered_file f1(jfd, buf1), f2(ufd, buf2);
    
  while(1) {
  
    bool s1white = false, s2white = false;
    
    int c1, c2;
    
    do {
      c1 = f1.get();
      
      if(isspace(c1)) s1white = true;
      
    } while(c1 != f1.FAIL && c1 && isspace(c1));
    
    do {
      c2 = f1.get();
      
      if(isspace(c2)) s1white = true;
      
    } while(c2 != f1.FAIL && c2 && isspace(c2));
    
    if(c2 == f2.FAIL || c1 == f1.FAIL) return -1;
      
    // there must either be no whitespace or both whitespace
    if(s1white != s2white)
      return 0;
    
    if(c1 != c2)
      return 0;
      
    // this is valid because if one is null and the other isnt, we will exit as above. If they are both null, we get here and exit
      // if neither is null, we continue
    if(!c1)
      return 1;
  }
}