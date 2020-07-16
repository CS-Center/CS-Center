#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include "file_access_checker.hpp"

using namespace std;

FileAccessChecker::init(vector<string> dirs, vector<string> readables, vector<string> writables) {
  
  dir_fs = dirs;
  read_fs = readables;
  write_fs = writables;
  
  return 0;
}

bool prefix_search(vector<string>& items, const string& elem) {
  for(string& s : items)
    if(s.size() <= elem.size() && elem.substr(0, s.size()) == s) 
      return 1;
      
  return 0;
}

bool linear_search(vector<string> items, const string& elem) {
  for(string& s : items)
    if(s == elem)
      return 1;
      
  return 0;
}

bool FileAccessChecker::check(const string& s, bool is_write) {
  if(is_write)
    return linear_search(write_fs, file);

  return linear_search(dir_fs, file, true) || linear_search(read_fs, file);
}

vector<string> base_dir_fs = { "/usr/", "/lib/", "/lib64/", "/proc/self/", };

vector<string> base_read_fs = { 
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
};

std::vector<const char*> base_write_fs = {
  "/dev/null", 
  
  "/dev/stdout",
  "/dev/stderr"
};