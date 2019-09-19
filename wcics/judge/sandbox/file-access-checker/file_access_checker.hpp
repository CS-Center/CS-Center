#pragma once

#include "file_access_trie.hpp"

class FileAccessChecker {
  // directories that can be read
  FileAccessTrie dir_trie;
  
  // readable and writable files, exact matches
  // linear searched since they are really small
  // may change in the future to binary search on sorted list, or set, or hash set
  const char* const* read_fs;
  const char* const* write_fs;
    
public:
  // initialize with string arrays
  // utils in utils/args.hpp
  // status will be filled in 0 on success, -1 otherwise
  FileAccessChecker(const char* const* dirs, const char* const* readables, const char* const* writables, int& status);
  
  bool check(const char* file, bool is_write);
};