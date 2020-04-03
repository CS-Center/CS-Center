#include <string.h>

#include "utils/files.hpp"
#include "checkers/utils/buffers.hpp"
#include "identical.hpp"

int identical_check(int jfd, int ufd, const char* arg) {

  int l1 = file_len(jfd), l2 = file_len(ufd);
  
  // check errors
  if(l1 < 0 || l2 < 0) return -1;
  
  // lol lengths aren't even right 
  if(l1 != l2) return 0;

  buffered_file f1(jfd, buf1), f2(ufd, buf2);
  
  int c1, c2;
  
  // iterate while both streams are good and no eof 
  // since they have the same length, either eof is sufficient
  // stop iterating if we ever don't match
  do {
    c1 = f1.get(), c2 = f2.get();
  } while(c1 != f1.FAIL && c2 != f2.FAIL && !f1.eof() && c1 == c2);
  
  if(c1 == f1.FAIL || c2 == f1.FAIL) return -1;
  
  // if we got to eof, we succeed, since both files have the same length
  return f1.eof();
}