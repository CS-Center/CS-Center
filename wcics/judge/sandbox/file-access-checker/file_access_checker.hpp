#pragma once

#include <string>
#include <vector>

class FileAccessChecker {
  
  // linear searched since they are really small
  // may change in the future to binary search on sorted list, or set, or hash set
  // Since almost all accesses are to /usr/, we put that first in dir_fs and search it first
  std::vector<std::string> dir_fs, read_fs, write_fs;
    
public:
  int init(std::vector<std::string> ds, std::vector<std::string> rs, std::vector<std::string> ws);
  
  bool check(std::string file, bool is_write);
};

extern std::vector<std::string> base_dir_fs, base_read_fs, base_write_fs;