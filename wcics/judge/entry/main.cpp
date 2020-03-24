#include <cstring>
#include <cstdlib>

#include "errors/return_codes.hpp"
#include "grading/main_grader.hpp"

int main(int argc, char ** argv) {
  if(argc == 1) return NO_ARGS;

  if(strcmp(argv[1], "grade") == 0) {
    if(argc == 2) return NO_SUBMISSION_ID;
  
    const char* num = argv[2];
    
    // reject long integer IDs
    if(strlen(num) > 18) return SUBMISSION_ID_TOO_LONG;
    
    // check that the whole string is only digits
    for(int i = 0; num[i] && i < 18; i++)
      if(num[i] > '9' || num[i] < '0')
        return SUBMISSION_ID_INVALID;
  
    // exit with the return code of the grade subfunction
    return grade(atoll(num));
  }
  
  // operation not understood
  return BAD_OPERATION;
}