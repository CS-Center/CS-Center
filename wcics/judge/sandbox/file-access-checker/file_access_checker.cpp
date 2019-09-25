#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include "file_access_checker.hpp"

FileAccessChecker::FileAccessChecker(const char* const* dirs, const char* const* readables, const char* const* writables, int& status) :
  dir_trie(dirs, status),
  read_fs(readables),
  write_fs(writables)
{}

bool linear_search(const char* const* arr, const char* elem) {
  for(; *arr; arr++) {
    if(strcmp(*arr, elem) == 0) {
      return 1;
    }
  }
  return 0;
}

bool FileAccessChecker::check(const char* file, bool is_write) {
  if(is_write)
    return linear_search(write_fs, file);
  
  bool ret = linear_search(read_fs, file);
  
  if(ret)
    return true;

  // if it errors, no problem, just pass it up the line
  return dir_trie.find(file);
}

std::vector<const char*> base_dir_fs = { "/usr/", "/lib/", "/proc/self/", };

std::vector<const char*> base_read_fs = { 
  "/usr", 
  "/lib", 
  "/proc/self",
  
  "/etc/ld.so.nohwcap",
  "/etc/ld.so.preload",
  "/etc/ld.so.cache",
  "/etc/localtime", 
  "/etc/passwd",
  "/etc/nsswitch.conf",
  
  "/dev/stdin",

  "/proc/meminfo",
  "/proc/cpuinfo",
  
  "/etc/os-release",
  "/proc/sys/vm/overcommit_memory",
};

std::vector<const char*> base_write_fs = {
  "/dev/null", 
  
  "/dev/stdout",
  "/dev/stderr"
};