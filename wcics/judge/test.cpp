#include <iostream>

// im lazy as hell
#include "sandbox/file-access-checker/file_access_trie.cpp"

using namespace std;

int main() {

  FileAccessTrie fat;
  
  const char* arr[] = {
    "/usr/",
    "/lib/",
    "/lib64/",
    0
  };
  
  const char* searched[] = {
    "/usr/",
    "/lib/",
    "/lib64/",
    "/opt/",
    "/proc/",
    "/etc/"
  };
  
  int len = 6;
  
  if(fat.init(arr)) { perror("fat.init"); return -1; }
  
  
  for(int i = 0; i < 1e7; i++) {
    const char* str = searched[i % 6];
    
    fat.find(str);
  }
}