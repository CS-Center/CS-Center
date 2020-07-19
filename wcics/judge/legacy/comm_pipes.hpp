#include "sandbox/config.hpp"
#include "sandbox/communicator.hpp"

#define PIPE_NULL 0
#define PIPE_INHERIT 1
#define PIPE_NORMAL 2

// make pipes
void make_pipes(Communicator& comm, config& conf, int pipe_stdin, int pipe_stdout, int pipe_stderr);