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

  static void pipe(scoped_fd& rd, scoped_fd& wr);

  // open a file (replace fd)
  void open(const char* file, int flags, int mode);
  
  // seek to a set position
  void seek(int);
  
  // ftruncate
  void trunc(int);
  
  // close the fd
  void close();
  
  int write(const char* buf, int len);
  int read(char* buf, int len);
  
  ~scoped_fd();
};