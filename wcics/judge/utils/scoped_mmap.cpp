#include <stdio.h>
#include <sys/mman.h>

#include "utils/files.hpp"

#include "scoped_mmap.hpp"

scoped_mmap::scoped_mmap() : ptr(0) {}

scoped_mmap::scoped_mmap(void* ptr) : ptr(ptr) {}

scoped_mmap::~scoped_mmap() {
	if(ptr && munmap(ptr, len))
		perror("scoped_mmap::~scoped_mmap");
}

int scoped_mmap::mmap_whole(int fd, int flen) {
	if(flen == -1) {
    len = file_len(fd);
    
    if(len < 0) {
		  perror("scoped_mmap::mmap_whole: file_len");
		  return -1;
	  }
  }
  
  else 
    len = flen;

	len++;

	ptr = mmap(0, len, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);

	if(ptr == MAP_FAILED) {
		ptr = 0;
		perror("scoped_mmap::mmap_whole: mmap");

		return -1;
	}

	return 0;
}
