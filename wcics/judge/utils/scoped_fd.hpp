#pragma once

// a scoped file descriptor class
// guarantees that it will try to close the file
// it will warn if it fails
struct scoped_fd {
  int fd;
  const char* name;
  
  // name is used for debugging
  scoped_fd(int fd, const char* name = "<unknown>");
  scoped_fd(const char* name = "<unknown>");
  
  scoped_fd(scoped_fd&& other);

  static int pipe(scoped_fd& rd, scoped_fd& wr);

  // open a file (replace fd)
  int open(const char* file, int flags, int mode);
  
  // seek to a set position
  int seek(int);
  
  // ftruncate
  int trunc(int);
  
  // close the fd
  int close();
  
  int write(const char* buf, int len);
  int read(char* buf, int len);
  
  ~scoped_fd();
};