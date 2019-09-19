#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

#include "paths.hpp"

int normalize_abs(const char* path, char buf[PATH_MAX]) {  
  // use a "stack" allocated on the program stack for maximum efficiency
	int slash_stack[PATH_MAX], sti = -1;
	if(path[0] != '/') {
    errno = EINVAL;
		return -1;	
	}
  
  // ensure length is valid
  if(strlen(path) >= PATH_MAX) {
    errno = ENAMETOOLONG;
    return -1;
  }

	buf[0] = '/';
	slash_stack[++sti] = 1;
	int wr_ind = 1, pre_slash = 0, i;
  
	for(i = 1; path[i]; i++) {
		// fwrite(buf, 1, wr_ind, stdout);
		// puts("");
		if(path[i] == '/') {
			int clen = i - pre_slash - 1;
			//fwrite(path + pre_slash + 1, 1, clen, stdout);
			//puts("");
			if(clen == 0 || (clen == 1 && strncmp(path + pre_slash + 1, ".", 1) == 0)) {
				wr_ind = slash_stack[sti];
				pre_slash = i;
				continue;
			}
			else if(clen == 2 && strncmp(path + pre_slash + 1, "..", 2) == 0) {
				pre_slash = i;
        
        // if we arent at the topmost slash, pop
				if(sti > 0) sti--;
        
        // in any case, reset the write index to the top of the stack
				wr_ind = slash_stack[sti];
				continue;
			}
			buf[wr_ind++] = '/';
			pre_slash = i;
			slash_stack[++sti] = wr_ind;
		}
		else
			buf[wr_ind++] = path[i];
	}
  
  int clen = i - pre_slash - 1;
  if(clen == 2 && strncmp(path + pre_slash + 1, "..", 2) == 0) {
    if(sti > 0) sti--;
    wr_ind = slash_stack[sti];
  }
  
  if(clen == 0 || (clen == 1 && strncmp(path + pre_slash + 1, ".", 1) == 0))
    wr_ind = slash_stack[sti];
  
	buf[wr_ind] = 0;
  return 0;
}

int fulljoin(const char* dir, const char* file, char buf[PATH_MAX]) {
	if(file[0] == '/')
		return normalize_abs(file, buf);
  
  // need that slash character
  if(strlen(dir) + 1 + strlen(file) >= PATH_MAX) {
    errno = ENAMETOOLONG;
    return -2;
  }
  
  // cat into a new buffer
  char buf2[PATH_MAX];
  strcpy(buf2, dir);
  strcat(buf2, "/");
  strcat(buf2, file);
  return normalize_abs(buf2, buf);
}

// testing code
/*
int main() {
	char p[PATH_MAX], f[PATH_MAX];
  scanf("%s %s", p, f);
  
	char buf[PATH_MAX];
	int ret = fulljoin(p, f, buf);
  if(ret) {
    fprintf(stderr, "fulljoin -> %d: ", ret);
    perror("");
    return -1;
  }
	puts(buf);
}*/