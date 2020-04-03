#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "utils/files.hpp"

int file_len(int fd) {
	struct stat buf;

	if(fstat(fd, &buf)) { return -1; }

	return buf.st_size;	
}
