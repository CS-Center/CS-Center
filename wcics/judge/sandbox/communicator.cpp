#include <sys/epoll.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <sched.h>
#include <linux/futex.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <sys/prctl.h>

#include "config.hpp"
#include "communicator.hpp"
#include "utils/pipes.hpp"

void func() __attribute__((constructor));

void func() {
  struct sigaction act;
  act.sa_handler = SIG_IGN;

  if(sigaction(SIGPIPE, &act, 0)) {
    perror("Failed to disable SIGPIPE");
    _exit(-1);
  }
}
    
inline int add_nonblock(int fd) {
  int flags = fcntl(fd, F_GETFL);
  
  if(flags == -1) {
    perror("Failed to get flags from fd");
    
    return -1;
  }
  
  if(fcntl(fd, F_SETFL, flags | O_NONBLOCK)) {
    perror("Failed to add non-blocking to fd");
    
    return -1;
  }
    
  return 0;
}

Communicator::Communicator(
  communicate_fd& infd, 
  communicate_fd& outfd, 
  communicate_fd& errfd,
  int& status
) :
  in(infd),
  out(outfd),
  err(errfd)
{
  status = init();
}

// TODO: move make_pipes code to here?
Communicator::Communicator(
  communicate_fd& infd, 
  communicate_fd& outfd, 
  communicate_fd& errfd,
  config& conf,
  int pipe_stdin,
  int pipe_stdout,
  int pipe_stderr,
  int& status
) :
  in(infd),
  out(outfd),
  err(errfd)
{
  status = make_pipes(*this, conf, pipe_stdin, pipe_stdout, pipe_stderr);
  
  if(status)
    return;
  
  status = init();
}

// create and init the epfd
int Communicator::init() {
  epfd = epoll_create1(EPOLL_CLOEXEC);

  if(epfd == -1) {
    perror("Communicator::init: Failed to create epoll instance");
    
    return -1;
    
  }
  
  struct epoll_event ev;
    
  if(in.fd != -1) {
    ev.events = EPOLLOUT;
    ev.data.fd = in.fd;
    
    // ensure that the fd wont block
    // we can now write large amounts of data without blocking
    if(add_nonblock(in.fd)) {
      fputs("Failed to add non-blocking flag to stdin\n", stderr);
      
      return -1;
    }
      
    if(epoll_ctl(epfd, EPOLL_CTL_ADD, in.fd, &ev)) {
      perror("Communicator::init: Failed to add stdin to epfd");
      
      return -1;
    }
  }
    
  if(out.fd != -1) {
    ev.events = EPOLLIN;
    ev.data.fd = out.fd;
    
    // make sure that we can make large reads without blocking
    if(add_nonblock(out.fd)) {
    
      fputs("Failed to add non-blocking flag to stdout\n", stderr);
      return -1;
    }
      
    if(epoll_ctl(epfd, EPOLL_CTL_ADD, out.fd, &ev)) {
      perror("Communicator::init: Failed to add stdout to epfd");
      
      return -1;
      
    }
  }
    
  if(err.fd != -1) {
    ev.events = EPOLLIN;
    ev.data.fd = err.fd;
    
    if(add_nonblock(err.fd)) {
      fputs("Failed to add non-blocking flag to stderr\n", stderr);
      return -1;
    }
      
    if(epoll_ctl(epfd, EPOLL_CTL_ADD, err.fd, &ev)) {
      perror("Communicator::init: Failed to add stderr to epfd");
      return -1;
    }
  }
  
  return 0;
}


int Communicator::launch(void) {
  char name[] = "communicator";
  
  prctl(PR_SET_NAME, name);

  // leave null byte
  int in_rem = in.length, out_rem = out.length - 1 , err_rem = err.length - 1;
  
  if(in.fd == -1)
    in_rem = 0;
    
  if(out.fd == -1)
    out_rem = 0;
    
  if(err.fd == -1)
    err_rem = 0;
    
  // stdin wont close if we give an fd but no input, so we close it here
  if(in_rem == 0 && in.fd != -1) {
    if(close(in.fd))
      perror("Communicator::launch: could not close empty stdin pipe");
    else
      in.fd = -1;
  }
  
  char* in_buf = 0, *out_buf = 0, *err_buf = 0;  
  if(in_rem)
    in_buf = in.buffer;
    
  if(out_rem) {
    out_buf = out.buffer;
    out_buf[0] = 0;
  }
  
  if(err_rem) {
    err_buf = err.buffer;
    err_buf[0] = 0;
  }
  
  epoll_event evs[3];
  
  while(in_rem || out_rem || err_rem) {
    int ev_cnt = epoll_wait(epfd, evs, 3, -1);
    
    if(ev_cnt == -1) {
      perror("Communicator::launch: epoll_wait");
      return -1;
    }
    
    while(ev_cnt--) {
      epoll_event& ev = evs[ev_cnt];
      if(ev.data.fd == in.fd && in_rem) {
        int bcnt;
        do {
          // write to their stdin
          bcnt = write(in.fd, in_buf, in_rem);
          if(bcnt == -1) {
            if(errno == EPIPE) {
              fputs("Warning: broken pipe on stdin!\n", stderr);

              in_rem = 0;

              if(epoll_ctl(epfd, EPOLL_CTL_DEL, in.fd, &ev))
                perror("Communicator: monitor: error removing stdin from interest list");

              break;
            }

            perror("error on stdin");

            if(errno == EWOULDBLOCK || errno == EAGAIN)
              break;

            perror("Communicator: monitor: stdin: write");
            return 1;
          }

          // move their buf and decrease their rem
          in_buf += bcnt;
          in_rem -= bcnt;

          // if we are done, close their fd
          if(in_rem == 0) {
            if(close(in.fd)) {
              // not critical
              perror("Communicator: monitor: error closing stdin fd");
            }
            else
              in.fd = -1;

          }
        } while(bcnt);
      }
      else if(ev.data.fd == out.fd) {
        int bcnt;
        do {
          if(out_rem == 0) {
            // tried to read too many bytes
            // OLE
            do_ole();
            return 0;
          }

          // read from their stdout
          bcnt = read(out.fd, out_buf, out_rem);
          if(bcnt == -1) {
            if(errno == EWOULDBLOCK || errno == EAGAIN)
              break;

            perror("Communicator: monitor: stdout: read");
            return 2;
          }

          if(bcnt == 0) {
            out_rem = 0;

            // remove stdout from interest list
            // if this fails, its not critical, so dont exit if it does
            if(epoll_ctl(epfd, EPOLL_CTL_DEL, out.fd, &ev))
              perror("Communicator: monitor: error removing stdout from interest list");

            if(close(out.fd)) {
              // not critical
              perror("Communicator: monitor: error closing stdout fd");
            }
            else
              out.fd = -1;

            break;
          }

          out_buf[bcnt] = 0;

          // move their buf and decrease their rem
          out_buf += bcnt;
          out_rem -= bcnt;
        } while(bcnt);
      }
      else if(ev.data.fd == err.fd) {
        int bcnt;
        do {
          if(err_rem == 0) {
            do_ole();
            return 0;
          }

          // read from their stderr
          bcnt = read(err.fd, err_buf, err_rem);
                    
          if(bcnt == -1) {
            if(errno == EWOULDBLOCK || errno == EAGAIN)
              break;

            perror("Communicator: monitor: stderr: read");
            return 2;
          }

          if(bcnt == 0) {
            err_rem = 0;

            // remove stderr from interest list
            // if this fails, its not critical, so dont exit if it does
            if(epoll_ctl(epfd, EPOLL_CTL_DEL, err.fd, &ev))
              perror("Communicator: monitor: error removing stderr from interest list");

            if(close(err.fd)) {
              // not critical
              perror("Communicator: monitor: error closing stderr fd");
            }
            else
              err.fd = -1;

            break;
          }

          err_buf[bcnt] = 0;

          // move their buf and decrease their rem
          err_buf += bcnt;
          err_rem -= bcnt;
          
        } while(bcnt);
      }
    }
  }
    
  return 0;
}

void Communicator::do_ole() {
  ole = true;
  // we dont close fds here because they will be closed in constructor, and closing pipes doesnt offer any benefit
    // since the process will be killed after this anyway
}

communicate_fd& Communicator::get_in() { return in; }
communicate_fd& Communicator::get_out() { return out; }
communicate_fd& Communicator::get_err() { return err; }

bool Communicator::is_ole() { return ole; }

Communicator::~Communicator() {
  if(close(epfd))
    perror("~Communicator: Error closing epoll instance");
    
  if(in.fd != -1 && close(in.fd))
    perror("~Communicator: Error closing stdin");
  else
    in.fd = -1;
    
  if(out.fd != -1 && close(out.fd))
    perror("~Communicator: Error closing stdout");
  else
    out.fd = -1;
    
  if(err.fd != -1 && close(err.fd))
    perror("~Communicator: Error closing stderr");
  else
    err.fd = -1;
}