#pragma once

#include <string>
#include <vector>

class FileAccessChecker {
  
  // linear searched since they are really small
  // may change in the future to binary search on sorted list, or set, or hash set
  // Since almost all accesses are to /usr/, we put that first in dir_fs and search it first
  const char* const* dir_fs;
  const char* const* read_fs;
  const char* const* write_fs;
    
public:
  FileAccessChecker(const char* const* ds, const char* const* rs, const char* const* ws);
  
  bool check(const char* file, bool is_write);
};

extern const char* const* base_dir_fs;
extern const char* const* base_read_fs;
extern const char* const* base_write_fs;