#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#include "utils/debug.hpp"

long get_proc_memory_field(pid_t p, const char* pat) {
  char buf[4096];
  snprintf(buf, 4095, "/proc/%d/status", p);
  
  int pat_len = strlen(pat);
  
  int fd = RUNTIME_FUNC(open(buf, O_RDONLY | O_CLOEXEC));
    
  int cnt;
  
  do {
    cnt = RUNTIME_FUNC(read(fd, buf, 4095));
    
    buf[cnt] = 0;
        
    char* cur = buf;
    while(cur) {    
      if(cur - buf + pat_len + 1 <= 4095 && strncmp(cur + 1, pat, pat_len) == 0) {
        long ret;
        sscanf(cur + 2 + pat_len, "%ld kB", &ret);
        
        RUNTIME_FUNC(close(fd));
        
        return ret * 1024L;
      }
      
      cur = (char*) memchr(cur + 1, '\n', 4095 - (cur - buf + 1));
    }
  } while(cnt);
  
  errno = EINVAL;
  
  perror("Could not find the required field when reading process memory amount");
  
  RUNTIME_FUNC(-1);
}