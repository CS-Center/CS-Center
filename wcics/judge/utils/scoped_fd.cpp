#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

#include "scoped_fd.hpp"

int scoped_fd::pipe(scoped_fd& rd, scoped_fd& wr) {
  int p[2];
  
  if(pipe2(p, O_CLOEXEC))
    return -1;

  rd.fd = p[0];
  wr.fd = p[1];
  
  return 0;
}

scoped_fd::scoped_fd(const char* name) : name(name), fd(-1) {}
scoped_fd::scoped_fd(int fd, const char* name) : fd(fd), name(name) {}

scoped_fd::scoped_fd(scoped_fd&& other) : fd(other.fd), name(other.name) {
  // invalidate other fd
  other.fd = -1;
};

int scoped_fd::open(const char* name, int flags, int mode) {
  fd = ::open(name, flags | O_CLOEXEC, mode);
  
  if(fd < 0)
    return fd;
    
  return 0;
}

int scoped_fd::seek(int pos) {
  return lseek(fd, pos, SEEK_SET) == -1;
}

int scoped_fd::trunc(int len) {
  return ftruncate(fd, len);
}

int scoped_fd::close() {
  if(fd == -1) return 0;
  
  int res = ::close(fd);
  
  if(res)
    return res;
    
  fd = -1;
  
  return 0;
}

int scoped_fd::write(const char* buf, int len) {
  int cnt = 0, offset = 0;
  
  do{
    offset += cnt;

    cnt = ::write(fd, buf + offset, len - offset);
  }
  while(cnt > 0 && len > offset);
  
  if(cnt < 0) return -1;
  
  return offset;
}

int scoped_fd::read(char* buf, int len) {
  int cnt = 0, offset = 0;
  
  do{
    offset += cnt;

    cnt = ::read(fd, buf + offset, len - offset);
  }
  while(cnt > 0 && len > offset);
  
  if(cnt < 0) return -1;
  
  return offset;
}

scoped_fd::~scoped_fd() {
  if(this->close()) {
    fprintf(stderr, "Could not close scoped_fd with name = ");
    
    perror(name);
  }
}