#include <stdio.h>

#include "tokenizer.hpp"

int check_tokens(const char* expected, const char* user, check_token_sig func, const char* arg) {
  int eoff, uoff, eres, ures, fres;
  
  char ebuf[4096], ubuf[4096];
  
  while(1) {
    // scanf both
    eres = sscanf(expected, "%4095s%n", ebuf, &eoff);
    
    ures = sscanf(user, "%4095s%n", ubuf, &uoff);
    
    expected += eoff;
    user += uoff;
    
    // if one got a token but other doesnt, we dont match
    if(eres != ures)
      return 0;
      
    // if both have EOF, we succeed 
    if(eres == EOF)
      return 1;
      
    // call func
    fres = func(ebuf, ubuf, arg);
    
    // if they didnt match, fail
    if(fres != 1)
      return fres;
  }
  
  return -1;
}