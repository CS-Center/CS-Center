#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/uio.h>
#include <string.h>

#include "secure_process.hpp"
#include "utils/paths.hpp"

// manage file access checking
int SecureProcess::file_access_check(int dirfd, ull strptr, int flags) {
  // if they want to create the file, just say no
  if(flags & O_CREAT)
    return EACCES;

  char path[PATH_MAX + 1];
  
  int ret = readpath(strptr, path);
  if(ret)
    return ret;
  
  bool is_write = false;
  
  // O_TRUNC is implemntation defined whether or not it works on read modes, so just assume it is bad :angry:
  if(flags & (O_RDWR | O_WRONLY | O_TRUNC))
    is_write = true;
  
  if(path[0] == '/') {
    // path is absolute
    // dont read dir
    // normalize path
    if(normalize_abs(path, path))
      return -1;
    
    if(!fac.check(path, is_write)) {
      // if they try to write to a file that they cant, EACCES
      if(is_write) {
        fprintf(stderr, "Denied write access to %s\n", path);
        return EACCES;
      }
        
      // if they try to read from file they cant, ENOENT
      fprintf(stderr, "Denied read access to %s\n", path);
      return ENOENT;
    }
    
    // fprintf(stderr, "Allowed %s access to %s\n", is_write ? "write" : "read", path);
    
    // allow file :D
    return 0;
  }
  
  // TODO: potentially cache CWD?
  // TODO: potentially store an fd referring to /proc/{pid}/fd?
  
  char dirbuf[PATH_MAX + 1];
  char fdname[PATH_MAX + 1];
  char fullname[PATH_MAX + 1];
  
  if(dirfd == AT_FDCWD)
    snprintf(fdname, PATH_MAX, "/proc/%d/cwd", pid);
  else
    snprintf(fdname, PATH_MAX, "/proc/%d/fd/%d", pid, dirfd);
    
  int cnt = readlink(fdname, dirbuf, PATH_MAX);
  if(cnt == -1) {
    if(errno == EACCES) {
      // this could be because the process was killed
      fputs("SecureProcess::file_access_check: Unable to access symlink!\n", stderr);
      return ENOENT;
    }
    
    // Not a symlink... ?
    if(errno == EINVAL) {
      snprintf(res.info, INFO_BUF_LEN, "SecureProcess::file_access_check: The file to read is not a symlink!\n");
      death_ie(0);
      return ENOENT;
    }
    
    if(errno == ENOENT) {
      fputs("SecureProcess::file_access_check: The symlink to read does not exist!\n", stderr);
      // This could be because dirfd was invalid, or the process was killed
      // assume the former
      return EBADF;
    }
  
    // Catch all :)
    death_ie("SecureProcess::file_access_check: readlink");
    return ENOENT;
  }
  
  dirbuf[cnt] = 0;
  if(fulljoin(dirbuf, path, fullname)) {
    if(errno == ENAMETOOLONG) {
      fputs("warning: file name too long to check!\n", stderr);
      return ENAMETOOLONG;
    }
    death_ie("SecureProcess::file_access_check: fulljoin");
    return ENOENT;
  }
  
  if(!fac.check(fullname, is_write)) {
    // if they try to write to a file that they cant, EACCES
    if(is_write) {
      fprintf(stderr, "Denied write access to %s\n", fullname);
      return EACCES;
    }

    // if they try to read from file they cant, ENOENT
    fprintf(stderr, "Denied read access to %s\n", fullname);
    return ENOENT;
  }

  // fprintf(stderr, "Allowed %s access to %s\n", is_write ? "write" : "read", fullname);

  // allow file :D
  return 0;
}

int SecureProcess::readpath(ull strptr, char path[PATH_MAX + 1]) {
  int to_read = PATH_MAX;
  path[PATH_MAX] = 0;

  int rcnt = 0;
  
  struct iovec local, remote;
  local.iov_base = path;
  local.iov_len = to_read;
  
  remote.iov_base = (void*)strptr;
  remote.iov_len = to_read;
  
  while(rcnt < to_read) {
    int ret = process_vm_readv(pid, &local, 1, &remote, 1, 0);
    
    // EFAULT means that the process literally decided to give us an invalid address
    // nice try buddy...
    if(ret == -1 && errno != EFAULT) {
      death_ie("SecureProcess::readpath: process_vm_readv");
      return -1;
    }
    
    // check for a null terminator
    if(memchr(local.iov_base, 0, ret))
      break;
    
    rcnt += ret;
    local.iov_base = (char*)local.iov_base + ret;
    local.iov_len -= ret;
    
    remote.iov_base = (char*)remote.iov_base + ret;
    remote.iov_len -= ret;
  }
  
  if(strlen(path) == PATH_MAX) {
    fputs("SecureProcess::readpath: Path too long!\n", stderr);
    return ENAMETOOLONG;
  }
  
  return 0;
}