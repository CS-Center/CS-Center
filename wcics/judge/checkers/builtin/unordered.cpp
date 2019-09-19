#include <vector>
#include <algorithm>
#include <string.h>
#include <stdio.h>

struct string_result {
  const char* ptr;
  int len;
  
  string_result(const char* ptr, int len) : ptr(ptr), len(len) {}
  
  bool operator<(const string_result& other) const {
    // printf("comparing %s with %s", ptr, other.ptr);
  
    int r1 = strncmp(ptr, other.ptr, std::min(len, other.len));
    
    // printf(": strncmp: %d\n", r1);
    
    // equal
    // check lengths 
    if(r1 == 0)
      return len < other.len;
      
    // otherwise, if <0, then yes, otherwise no
    return r1 < 0;
  };
  
  bool operator==(const string_result& other) const {
    return len == other.len && strncmp(ptr, other.ptr, len) == 0;
  };
};

int unordered_check(const char* expected, const char* user, const char* arg) {
  int eoff, uoff, eres, ures, elen, ulen;
  
  char ebuf[4096], ubuf[4096];
  
  std::vector<string_result> etkns, utkns;
  
  while(1) {
    // scanf both
    eres = sscanf(expected, "%4095s%n", ebuf, &eoff);
    
    ures = sscanf(user, "%4095s%n", ubuf, &uoff);
    
    expected += eoff;
    user += uoff;
    
    // if one got a token but other doesnt, we dont match
    if(eres != ures)
      return 0;
      
    // if both have EOF, we break 
    if(eres == EOF)
      break;
      
    // add tokens
    elen = strlen(ebuf);
    ulen = strlen(ubuf);
    
    etkns.emplace_back(expected - elen, elen);
    utkns.emplace_back(user - ulen, ulen);
  }
  
  // order them
  std::sort(etkns.begin(), etkns.end());
  std::sort(utkns.begin(), utkns.end());
  
  // if they are equal, then yes, otherwise no
  return etkns == utkns;
}