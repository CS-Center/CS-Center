#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

long get_proc_memory_field(pid_t p, const char* pat) {
  char buf[4096];
  snprintf(buf, 4095, "/proc/%d/status", p);
  
  int pat_len = strlen(pat);
  
  int fd = open(buf, O_RDONLY | O_CLOEXEC);
  
  if(fd == -1)
    return -1;
    
  int cnt;
  
  do {
    cnt = read(fd, buf, 4095);
    
    if(cnt == -1) {
      if(close(fd))
        perror("Warning: error closing fd in get_proc_memory_field");
      perror("Warning: error reading from fd in get_proc_memory_field");
      return -1;
    }
    
    buf[cnt] = 0;
        
    char* cur = buf;
    while(cur) {    
      if(cur - buf + pat_len + 1 <= 4095 && strncmp(cur + 1, pat, pat_len) == 0) {
        long ret;
        sscanf(cur + 2 + pat_len, "%ld kB", &ret);
        
        if(close(fd))
          perror("Could not close fd before returning from proc memory reading");
        
        return ret * 1024L;
      }
      
      cur = (char*) memchr(cur + 1, '\n', 4095 - (cur - buf + 1));
    }
  } while(cnt);
  
  fputs("Warning: could not read field from proc fs\n", stderr);
  
  if(close(fd))
    perror("Could not close fd before returning from failed proc memory reading");
  
  return -1;
}