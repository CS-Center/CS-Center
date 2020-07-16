#pragma once

#include "sandbox/config.hpp"
#include "sandbox/communicator.hpp"

// null fds to be shared, for convenience
extern int null_write_fd, null_read_fd;

// make a pipe by references
void make_pipe(int& read_fd, int& write_fd);

#define PIPE_NULL 0
#define PIPE_INHERIT 1
#define PIPE_NORMAL 2

// make pipes
// removing?
// void make_pipes(Communicator& comm, config& conf, int pipe_stdin, int pipe_stdout, int pipe_stderr);