#pragma once

#include <sys/types.h>

#include "utils/async_launcher.hpp"
#include "config.hpp"

// a structure to represent a file descriptor
// used by the Communicator
// If fd represents stdin, buffer contains the data to write
  // and length contains the length of said buffer
// otherwise, buffer contains the output buffer
  // and length is the limit
// if fd is -1, the fd will be ignored
struct communicate_fd {
  int fd;
  char* buffer;
  int length;
};

// A class for monitoring and managing file descriptors
// Supports stdin, stdout & stderr
class Communicator {
private:
  // The epoll file descriptor
  int epfd;
  
  // whether or not process has done OLE
  bool ole;
  
  // the three file descriptors
  // infd shall be open for writing
  // outfd & errfd shall be open for reading
  // the lengths of out and err can be the exact size, the null byte will be appended
  communicate_fd &in, &out, &err;
    
public:
  Communicator(
    communicate_fd&, 
    communicate_fd&, 
    communicate_fd&,
    int& status
  );
  
  Communicator(
    communicate_fd& infd, 
    communicate_fd& outfd, 
    communicate_fd& errfd,
    config& conf,
    int pipe_stdin,
    int pipe_stdout, 
    int pipe_stderr,
    int& status
  );
  
  int init();
  
  // getters for in, out and err
  // used by make_comms
  communicate_fd& get_in();
  communicate_fd& get_out();
  communicate_fd& get_err();
  
  void do_ole();
  
  int launch(void);
  
  bool is_ole();
  
  ~Communicator();
};

using AsyncCommunicator = AsyncLauncher<Communicator>;