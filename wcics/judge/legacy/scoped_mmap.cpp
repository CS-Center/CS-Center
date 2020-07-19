#include <stdio.h>
#include <sys/mman.h>
#include "utils/debug.hpp"

#include "utils/files.hpp"

#include "scoped_mmap.hpp"

scoped_mmap::scoped_mmap() : ptr(0) {}

scoped_mmap::scoped_mmap(void* ptr) : ptr(ptr) {}

scoped_mmap::~scoped_mmap() {
	if(ptr)
		RUNTIME_FUNC(munmap(ptr, len));
}

scoped_mmap::scoped_mmap(int fd, int flen) {
	if(flen == -1) {
    PUSH_STACK(len = file_len(fd));
  }
  
  else 
    len = flen;

	len++;

	ptr = RUNTIME_FUNC_EQ(mmap(0, len, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0), MAP_FAILED);
}
