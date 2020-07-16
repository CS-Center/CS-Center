#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include "file_access_checker.hpp"

using namespace std;

FileAccessChecker::FileAccessChecker(const char* const* dirs, const char* const* readables, const char* const* writables) {
  
  dir_fs = dirs;
  read_fs = readables;
  write_fs = writables;
}

bool prefix_search(const char* const* items, const char* elem) {
  for(; *items; items++) {
    int l = strlen(*items);
    
    if(strncmp(*items, elem, l) == 0)
      return 1;
  }
  
  return 0;
}

bool linear_search(const char* const* items, const char* elem) {
  for(; *items; items++)
    if(strcmp(*items, elem) == 0) 
      return 1;
      
  return 0;
}

bool FileAccessChecker::check(const char* file, bool is_write) {
  if(is_write)
    return linear_search(write_fs, file);

  return prefix_search(dir_fs, file) || linear_search(read_fs, file);
}

const char* const _base_dir_fs[] = { "/usr/", "/lib/", "/lib64/", "/proc/self/", 0 };

const char* const _base_read_fs[] = { 
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
  "/dev/null",

  "/proc/meminfo",
  "/proc/cpuinfo",
  
  "/etc/os-release",
  "/proc/sys/vm/overcommit_memory",
  0
};

const char* const _base_write_fs[] = {
  "/dev/null", 
  
  "/dev/stdout",
  "/dev/stderr",
  0
};

const char* const* base_dir_fs = _base_dir_fs;
const char* const* base_read_fs = _base_read_fs;
const char* const* base_write_fs = _base_write_fs;