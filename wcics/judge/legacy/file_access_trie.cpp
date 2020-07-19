#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <new>

#include "file_access_trie.hpp"

FAT_node::FAT_node() : is_end(false) {
  // make children all null
  memset(children, 0, sizeof children);
}

FAT_node::FAT_node(const FAT_node& other) {
  is_end = other.is_end;
  
  for(int i = 0; i < 256; i++) {
    if(other.children[i])
      children[i] = new FAT_node(*other.children[i]);
    else
      children[i] = 0;
  }
}

// 0 on success, -1 on error
int FAT_insert(FAT_node** tree, const char* str) { 
  while(*str) {
    if(!*tree) {
      *tree = new (std::nothrow) FAT_node();
      if(*tree == 0) {
        errno = ENOMEM;
        return -1;
      }
    }
    
    // gets the tree's next node and loads its address into the var tree
    // make sure that we only take the lowest 8 bytes of the string
    
    tree = &((*tree)->children[int(*str) & 255]);
    str++;
  }
  
  if(!*tree) {
    *tree = new (std::nothrow) FAT_node();
    if(*tree == 0) {
      errno = ENOMEM;
      return -1;
    }
  }
  (*tree)->is_end = true;
  
  return 0;
}

bool FAT_find(FAT_node* tree, const char* str) {
  while(tree) {    
    if(tree->is_end) return 1;
    if(!*str) return 0;
    tree = tree->children[int(*str) & 255];
    str++;
  }
  
  return 0;
}

void FAT_del(FAT_node* tree) {
  if(!tree)
    return;
  for(int i = 0; i < 256; i++)
    FAT_del(tree->children[i]);
    
  delete tree;
}

FileAccessTrie::FileAccessTrie() : root(0) {}

int FileAccessTrie::init(const char* const* arr) {
  for(; *arr; arr++) {
    if(FAT_insert(&root, *arr)) {
      return -1;
    }
  }
  
  return 0;
}

bool FileAccessTrie::find(const char* str) {
  return FAT_find(root, str);
}

FileAccessTrie::~FileAccessTrie() {
  FAT_del(root);
}