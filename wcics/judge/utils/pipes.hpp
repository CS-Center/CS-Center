#pragma once

// null fds to be shared, for convenience
extern int null_write_fd, null_read_fd;

// make a pipe by references
void make_pipe(int& read_fd, int& write_fd);
