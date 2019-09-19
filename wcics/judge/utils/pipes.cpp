#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

#include "pipes.hpp"

int get_null_read_fd() {
  // if we cant open the null fd, what should we do?
  // we decide to return an invalid fd and print a loud warning
  
  int fd = open("/dev/null", O_RDONLY | O_CLOEXEC);
  
  if(fd == -1) {
    perror("\n\nWARNING: COULD NOT OPEN /dev/null FOR READING");
    fputs("\n\n", stderr);
  }
  
  return fd;
}

int get_null_write_fd() {
  int fd = open("/dev/null", O_WRONLY | O_CLOEXEC);
  
  if(fd == -1) {
    perror("\n\nWARNING: COULD NOT OPEN /dev/null FOR WRITING");
    fputs("\n\n", stderr);
  }
  
  return fd;
}

int null_read_fd = get_null_read_fd();
int null_write_fd = get_null_write_fd();

int make_pipe(int& read_fd, int& write_fd) {
  int p[2];
  if(pipe2(p, O_CLOEXEC))
    return -1;
  
  read_fd = p[0];
  write_fd = p[1];
  
  return 0;
}

inline bool validate_comm(int flag) {
  return flag == PIPE_NULL || flag == PIPE_INHERIT || flag == PIPE_NORMAL;
}

int make_pipes(Communicator& comm, config& conf, int pipe_stdin, int pipe_stdout, int pipe_stderr) {
  if(!validate_comm(pipe_stdin) || !validate_comm(pipe_stdout) || !validate_comm(pipe_stderr)) {
    errno = EINVAL;
    return -1;
  }

  if(pipe_stdin == PIPE_NULL) {
    // null stdin
    // give communicator -1 and conf a null read fd
    comm.get_in().fd = -1;
    conf.pstdin = null_read_fd;
  }
  
  else if(pipe_stdin == PIPE_INHERIT) {
    // inherit
    comm.get_in().fd = conf.pstdin = -1;
  }
  
  else {
    if(make_pipe(conf.pstdin, comm.get_in().fd))
      return -1;
  }
  
  // stdout
  if(pipe_stdout == PIPE_NULL) {
    // null stdout
    // give communicator -1 and conf a null write fd
    comm.get_out().fd = -1;
    conf.pstdout = null_write_fd;
  }
  
  else if(pipe_stdout == PIPE_INHERIT) {
    // inherit
    comm.get_out().fd = conf.pstdout = -1;
  }
  
  else {
    if(make_pipe(comm.get_out().fd, conf.pstdout))
      return -1;
  }
  
  // stderr
  if(pipe_stderr == PIPE_NULL) {
    // null stderr
    // give communicator -1 and conf a null write fd
    comm.get_err().fd = -1;
    conf.pstderr = null_write_fd;
  }
  
  else if(pipe_stderr == PIPE_INHERIT) {
    // inherit
    comm.get_err().fd = conf.pstderr = -1;
  }
  
  else {
    if(make_pipe(comm.get_err().fd, conf.pstderr))
      return -1;
  }
  
  return 0;
}