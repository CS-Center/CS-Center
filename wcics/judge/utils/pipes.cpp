#include <unistd.h>
#include <fcntl.h>

#include "pipes.hpp"
#include "utils/debug.hpp"

int get_null_read_fd() {
  int fd = RUNTIME_FUNC(open("/dev/null", O_RDONLY | O_CLOEXEC));
  
  return fd;
}

int get_null_write_fd() {
  int fd = RUNTIME_FUNC(open("/dev/null", O_WRONLY | O_CLOEXEC));
  
  return fd;
}

int null_read_fd = get_null_read_fd();
int null_write_fd = get_null_write_fd();

void make_pipe(int& read_fd, int& write_fd) {
  int p[2];
  
  RUNTIME_FUNC(pipe2(p, O_CLOEXEC));
  
  read_fd = p[0];
  write_fd = p[1];
}