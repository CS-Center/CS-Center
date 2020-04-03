#include <ctype.h>

#include "checkers/utils/whole_file_check.hpp"
#include "tokenizer.hpp"

struct token_arg {
  check_token_sig func;
  const char* arg;
};


int _check_tokens(char* judge_out, char* user_out, const void* varg) {

  const token_arg * ta = (const token_arg*) varg;

  auto func = ta->func;
  auto arg = ta->arg;
  
  while(*judge_out && *user_out) {
  
    // search for non-whitespace
    
    while(isspace(*judge_out) && *judge_out) judge_out++;
    
    // search for whitespace
    char* jtkn = judge_out;
    
    while(!isspace(*judge_out) && *judge_out) judge_out++;
    
    bool jeof = jtkn == judge_out;
    
    // search for non-whitespace
    while(isspace(*user_out) && *user_out) user_out++;
    
    char* utkn = user_out;
    
    while(!isspace(*user_out) && *user_out) user_out++;
    
    bool ueof = utkn == user_out;
    
    // one eof and the other didn't, so fail
    if(ueof != jeof) return 0;
    
    // both EOF, so correct
    if(ueof && jeof) return 1;
    
    // neither EOF
    
    // since the func requires a null terminated string, we null terminate it here and then re-write the old character back.
    char jcur = *judge_out;
    char ucur = *user_out;
    
    *judge_out = 0;
    *user_out = 0;
    
    // failed.
    if(!func(judge_out, user_out)) return 1;
    
    // replace old chars
    *judge_out = jcur;
    *user_out = ucur;
  }
  
  // both EOF, so we return success
  return 1;
}

int check_tokens(int jfd, int ufd, check_token_sig func, const char* arg) {
  token_arg ta;
  
  ta.func = func;
  ta.arg = arg;
  
  return whole_file_check(jfd, ufd, _check_tokens, &ta);
}