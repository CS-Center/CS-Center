#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>

#include "utils/scoped_mmap.hpp"
#include "utils/files.hpp"
#include "checkers/utils/buffers.hpp"

#include "whole_file_check.hpp"

int whole_file_check(int jfd, int ufd, whole_check_sig func, const void* arg) {
  scoped_mmap judge_m;
  
  char* judge_buf;
  
  int l1 = file_len(jfd);
  if(l1 < 0) {
    perror("whole_file_check: file_len judge");
  }
  
  if(l1 <= BUF_SIZE) {
    int offset = 0, cnt = 0;
    do {
      offset += cnt;
      
      cnt = read(jfd, buf1 + offset, l1 - offset);
    } while(cnt > 0);
    
    if(cnt < 0) {
      perror("whole_file_check: read judge");
      return -1;
    }
    
    judge_buf = buf1;
  }
  else {
    if(judge_m.mmap_whole(jfd, l1)) {
      perror("whole_file_check: mmap judge");
      return -1;
    }
    
    judge_buf = (char*) judge_m.ptr;
  }
  judge_buf[l1] = 0;
  

  int l2 = file_len(ufd);
  if(l2 < 0) {
    perror("unordered_check: file_len user");
  }
  
  int ret;
  if(l2 <= BUF_SIZE) {
    int offset = 0, cnt = 0;
    do {
      offset += cnt;
      
      cnt = read(ufd, buf2 + offset, l1 - offset);
    } while(cnt > 0);
    
    if(cnt < 0) {
      perror("unordered_check: read user");
      return -1;
    }
    
    return func(judge_buf, buf2, arg);
  }
  else {
    scoped_mmap user_m;
    
    if(user_m.mmap_whole(ufd, l2)) {
      perror("whole_file_check: mmap user");
      return -1;
    }
    
    return func(judge_buf, (char*) user_m.ptr, arg);
  }
}