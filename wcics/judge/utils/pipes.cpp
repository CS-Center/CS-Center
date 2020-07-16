#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

#include "pipes.hpp"

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

inline bool validate_comm(int flag) {
  return flag == PIPE_NULL || flag == PIPE_INHERIT || flag == PIPE_NORMAL;
}


void make_pipes(Communicator& comm, file_config& conf, int pipe_stdin, int pipe_stdout, int pipe_stderr) {
  if(!validate_comm(pipe_stdin) || !validate_comm(pipe_stdout) || !validate_comm(pipe_stderr)) {
    errno = EINVAL;
    RUNTIME_FUNC(-1);
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
    PUSH_STACK(make_pipe(conf.pstdin, comm.get_in().fd));
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
    PUSH_STACK(make_pipe(comm.get_out().fd, conf.pstdout));
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
    PUSH_STACK(make_pipe(comm.get_err().fd, conf.pstderr));
  }
}