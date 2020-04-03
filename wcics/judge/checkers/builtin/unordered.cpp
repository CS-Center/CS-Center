#include <vector>
#include <algorithm>
#include <string.h>
#include <ctype.h>

#include "checkers/utils/whole_file_check.hpp"

#include "unordered.hpp"

struct string_result {
  const char* ptr;
  int len;
  
  string_result(const char* ptr, int len) : ptr(ptr), len(len) {}
  
  bool operator<(const string_result& other) const {
    // sort by lengths because it's faster
    
    if(len < other.len) return 1;
    if(len > other.len) return 0;
    
    // same length, compare lex
    return strncmp(ptr, other.ptr, len) < 0;
  };
  
  bool operator==(const string_result& other) const {
    return len == other.len && strncmp(ptr, other.ptr, len) == 0;
  };
};

void tokenize(char* buffer, std::vector<string_result>& vec) {
  while(*buffer) {
    
    // next non-whitespace
    while(isspace(*buffer) && *buffer) buffer++;

    char* start = buffer;
    
    // next whitespace
    while(!isspace(*buffer) && *buffer) buffer++;
    
    // buffer == start if we hit EOF
    if(buffer != start) {
      vec.emplace_back(start, buffer - start);
    }
  }
}

int _unordered_check(char* judge_out, char* user_out, const void* arg) {
  std::vector<string_result> jtkns, utkns;
  
  tokenize(judge_out, jtkns);
  tokenize(user_out, utkns);
  
  // short circuit - if sizes don't match, don't bother sorting
  if(jtkns.size() != utkns.size()) return 0;
  
  // order them
  std::sort(jtkns.begin(), jtkns.end());
  std::sort(utkns.begin(), utkns.end());
  
  // if they are equal, then yes, otherwise no
  return jtkns == utkns;
}

int unordered_check(int jfd, int ufd, const char* arg) {
  return whole_file_check(jfd, ufd, _unordered_check, arg);
}