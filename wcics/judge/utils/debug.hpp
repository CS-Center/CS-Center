#pragma once

#include <stdexcept>
#include <stdio.h>
#include <errno.h>
#include <exception>
using namespace std;

inline void do_error(const char* file, const char* expr, int line, const char* func) {
  static const char* msg = "Runtime Error in file '%s' on line %d in function '%s' when executing '%s'.";

  fprintf(stderr, msg, file, line, func, expr);
  fprintf(stderr, "\nErrno is %d", errno);
  perror(", error message");

  if(uncaught_exception()) {
    fputs("Cannot throw here, an active exception has already been thrown!\n\n", stderr);
  }
  else {    
    fputs("\n", stderr);
    throw runtime_error("Abnormal termination after bad return value from a function. See traceback above.");
  }
}

inline int do_runtime_func(int r, const char* file, const char* expr, int line, const char* func) {  
  if(r < 0)
    do_error(file, expr, line, func);

  return r;
}

template<typename T>
inline T do_runtime_func_eq(T r, T v, const char* file, const char* expr, int line, const char* func) {  
  if(r == v)
    do_error(file, expr, line, func);

  return r;
}

template<typename T>
inline T do_runtime_func_neq(T r, T v, const char* file, const char* expr, int line, const char* func) {  
  if(r != v)
    do_error(file, expr, line, func);

  return r;
}

inline void print_stack_elem(const char* file, const char* fn, int line, const char* func) {
  static const char* msg = "Error caught in file '%s' on line %d in function '%s' when executing '%s'.\n\n";

  fprintf(stderr, msg, file, line, func, fn);
}

#define RUNTIME_FUNC(expr) do_runtime_func(expr, __FILE__, #expr, __LINE__, __PRETTY_FUNCTION__)
#define RUNTIME_FUNC_EQ(expr, val) do_runtime_func_eq(expr, val, __FILE__, #expr, __LINE__, __PRETTY_FUNCTION__)
#define RUNTIME_FUNC_NEQ(expr, val) do_runtime_func_neq(expr, val, __FILE__, #expr, __LINE__, __PRETTY_FUNCTION__)

#define PUSH_STACK(st) try{st;}catch(exception& e){print_stack_elem(__FILE__, #st, __LINE__, __PRETTY_FUNCTION__);throw e;}