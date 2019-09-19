#include <string.h>

#include "identical.hpp"

int identical_check(const char* expected, const char* user, const char* args) {
  // pretty easy, if they are identical, its good, if not, fail
  return strcmp(expected, user) == 0;
}