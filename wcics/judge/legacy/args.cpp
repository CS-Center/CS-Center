#include "args.hpp"

int arrlen(const char* const* arr) {
  int x = 0;
  while(arr[x]) 
    x++;
  return x;
}

void copy_args(const char* const* args, const char** &buf) {
  while(*args) {
    *buf = *args;
    buf++;
    args++;
  }
}

void cat_args(const char* const* args1, const char* const* args2, const char** buf) {
  copy_args(args1, buf);
  copy_args(args2, buf);
    
  *buf = 0;
}

// increment pointers instead of using an index variable
void add_executable(const char* file, const char* const* args, const char** buf) {
  *buf = file;
  buf++;
  
  copy_args(args, buf);
  *buf = 0;
}

char* _blank_args[] = {0};
char** blank_args = _blank_args;