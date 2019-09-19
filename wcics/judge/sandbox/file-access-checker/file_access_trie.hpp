#pragma once

struct FAT_node {
  FAT_node* children[256];
  
  bool is_end;
  
  FAT_node();
};

class FileAccessTrie {
  FAT_node* root;

public:
  FileAccessTrie();
  ~FileAccessTrie();

  // status will be 0 on success, -1 and errno on error
  FileAccessTrie(const char* const*, int& status);
  
  // 1 if the given string has a prefix in the collection, 0 otherwise
  bool find(const char* str);
};