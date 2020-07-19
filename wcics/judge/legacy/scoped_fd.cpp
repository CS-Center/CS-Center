#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

#include "utils/debug.hpp"

#include "scoped_fd.hpp"

void scoped_fd::pipe(scoped_fd& rd, scoped_fd& wr) {
  int p[2];
  
  RUNTIME_FUNC(pipe2(p, O_CLOEXEC));

  rd.fd = p[0];
  wr.fd = p[1];
}

scoped_fd::scoped_fd(const char* name) : name(name), fd(-1) {}
scoped_fd::scoped_fd(int fd, const char* name) : fd(fd), name(name) {}

scoped_fd::scoped_fd(scoped_fd&& other) : fd(other.fd), name(other.name) {
  // invalidate other fd
  other.fd = -1;
};

void scoped_fd::open(const char* name, int flags, int mode) {
  fd = RUNTIME_FUNC(::open(name, flags | O_CLOEXEC, mode));
}

void scoped_fd::seek(int pos) {
  RUNTIME_FUNC(lseek(fd, pos, SEEK_SET));
}

void scoped_fd::trunc(int len) {
  RUNTIME_FUNC(ftruncate(fd, len));
}

void scoped_fd::close() {
  if(fd != -1) {
    RUNTIME_FUNC(::close(fd));

    fd = -1;
  }
}

int scoped_fd::write(const char* buf, int len) {
  int cnt = 0, offset = 0;
  
  do{
    offset += cnt;

    cnt = RUNTIME_FUNC(::write(fd, buf + offset, len - offset));
  }
  while(cnt > 0 && len > offset);
    
  return offset;
}

int scoped_fd::read(char* buf, int len) {
  int cnt = 0, offset = 0;
  
  do{
    offset += cnt;

    cnt = RUNTIME_FUNC(::read(fd, buf + offset, len - offset));
  }
  while(cnt > 0 && len > offset);
    
  return offset;
}

scoped_fd::~scoped_fd() {
  this->close();
}