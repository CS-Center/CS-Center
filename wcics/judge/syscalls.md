# Syscall List

Each item has the syscall and relevant notes

## Read: read bytes from FD
  read(fd, buffer, cnt) = cnt\_read

  Notes:
  - Very common syscall
  - Use seccomp to optimize
  - Harmless

## Write: write bytes to FD
  write(fd, buffer, cnt) = cnt\_written
  
  Notes:
  - Very common 
  - Seccomp to optimize
  - Harmless so long as FD refers to stderr/stdout
  
## Open: open a file
  open(path, flags, mode) = fd
  
  Notes:
  - Common enough
  - Cannot be optimized with seccomp
  - Restrict allowed file openings
  - 3 Access modes
    - O\_RDONLY : allowed always
    - O\_WRONLY : disallowed always
    - O\_RDWR : disallowed for security
  - O\_APPEND : only useful if they can write, so NOPE
  - O\_ASYNC : generate signal when I/O is allowed on a file
  - O\_CLOEXEC : close the file descriptor on exec
    - Harmless, and often used
  - O\_CREAT : create file if it does not exist
    - May be required for compilers?
    - For normal programs, should be disallowed
    - mode must be specified if this flag is included
  - O\_DIRECT : do not buffer
    - This is a performance hit, so it is rarely useful
    - Seems harmless
  - O\_DIRECTORY : Fail if the target is not a directory 
    - Some programs need to open directories
  - O\_DSYNC : write operations will be transferred to hardware before write() (or similar) returns
    - Not applicable, since we don't allow writing
    - _Don't_ filter it, just a performance hit
  - O\_EXCL : ensure that the open creates a file
    - O\_CREAT is disallowed, so this won't be helpful anyway
  - O\_LARGEFILE : Allow files with sizes larger than 2 ^ 32 to be opened
    - should be harmless
  - O\_NOATIME : don't update the file's last access time
    - I don't care?
    - Not possible unless they own the file
    - Which shouldn't happen
  - O\_NOCTTY : 
    - If the related file is a terminal device, do not make it the processes controlling terminal, even if the process does not have one
    - Open up two terminals
      - In A, type `tty`
      - In B, echo some characters into the result of above
      - The characters will appear on A
    - TL;DR seems mostly harmless
  - O\_NOFOLLOW : Don't follow symlinks
    - Harmless?
  - O\_NONBLOCK & O\_NDELAY: Don't block on I/O operations
    - Harmless?
  - O\_PATH : indicates a location in the filesystem, but isn't really a file or anything
    - Basically, I exist, but the file isn't opened
    - Useful for directories with like fchdir, fstat
    - Useful if you have execute perms but not read perms and want to chdir or similar
    - Probably harmless
  - O\_SYNC : Similar to O\_DSYNC
    - Flushes writes and metadata, DYSNC does not flush metadata unless it must
  - O\_TMPFILE : Create unnamed temporary regular file
    - No, you cannot create files
    - Requires O\_WRONLY or O\_RDWR, both are disallowed
  - O\_TRUNC : If the file exists and is opened in write mode, it will be truncated to length 0
    - Since write mode is required, it is already killed
    - However, specifying this with READ is unspecified behaviour, so diable this flag always
    
## Close : close an open file descriptor
  close(fd) = status
  - Harmless
  - Speed up with seccomp
  
## Stat : return info about a file
  stat(filename, stat\_struct) = status
  - Getting info about a file is not that helpful...
  - However, letting runtimes stat a file that they can't open breaks them
    - Credit to @xyene
  
## FStat : return info about a file descriptor
  fstat(fd, stat\_struct) = status
  - Same as stat, but use fd instead of filename
  
## LStat : return info about a symlink
  lstat(filename, stat\_struct) = status
  - Same as stat, but returns info about the link (if it is a link), not the file the link refers to
  
## Poll : wait for event on FD
  poll(fds\_and\_events, number\_of\_fds, timeout) = count
  - Wait for events on FDs
  - Specifies what events occurred on the file descriptors, be it data ready to read, data can be written, error occurred...
  - Harmless, since it requires open fds, which are assumed to be valid
  - Seccomp
  
## LSeek : move file offset
  lseek(fd, offset, mode) = new\_offset
  - Move offset of file descriptor
  - Harmless
  - Seccomp
  
## MMap : map file into memory
  mmap(address, length, protection, flags, fd, offset) = pointer
  - Probably harmless
  - USeful to speed up file management
  - probably seccomp
  
## MProtect : protect region of memory
  mprotect(address, length, protection) = status
  - Protect region from being acccessed in a way that we don't like
  - This is one way for SIGSEGV to happen; you accessed protected memory
  - Used frequently by simple GCC programs
  - Probably harmless
  - probably seccomp
  
## MUnMap : unmap file from memory
  munmap(address, length) = status
  - Probably harmless
  - Inverse of mmap
  - probably seccomp
  
## Brk : change data segment size
  brk(address) = address
  - Change the data segment break value
  - Read: allocate data on the heap
  - again, fairly harmless
  - probably seccomp
  
## rt\_sigaction : manage signal handlers
  rt\_sigaction(signal, new\_action, old\_action) = status
  - probably harmless
  - rt\_sigaction is the syscall, sigaction is the glibc function
  - rt\_sigaction is used to provide 64-bit integers
  - probably seccomp
  
## rt\_sigprocmask : change blocked signals
  rt\_sigprocmask(option, signals, oldsignals, signal\_size) = status
  - Block signals
  - Great, you blocked signals...
  - Probably Seccomp
  
## rt\_sigreturn : return from signal handler
  rt\_sigreturn() = ?
  - Return from signal handler
  - Should not be called directly
  - Probably seccomp
  
## ioctl : modify device parameters
  ioctl(fd, request, ...) = output
  - A very, very generic call for modifying hardware
  - ??
  - Probably seccomp
  
## pread64 : read from file at offset
  read(fd, buf, count, offset) = count
  - like read, but specifies offset
  
## pwrite64 : write to file at offset
  write(fd, buf, count, offset) = count
  - like write, but specifies offset
  
## readv : read into multiple buffers
  readv(fd, buffers, buffercnt) = count
  - like read, but reads into multiple buffers
  
## writev : write from multiple buffers
  writev(fd, buffers, buffercnt) = cnt
  - like write, but writes from multiple buffers
  
## access : check for file & permissions
  access(pathname, mode) = status
  - similar to open, check if file is allowed
  
## pipe : create an interprocess communication channel
  pipe() = fd
  - Not likely to be used, ever
  - However, a random python program used it once and I reported it
  - regardless, it's probably harmless
  - seccomp
  
## select : wait for events on fds
  select(num, readfds, writefds, exceptfds, timeout) = count
  - This is used to implement cross-platform sleep
    - For some reason?
  - Probably seccomp
  
## sched\_yield : give up the processor to someone else
  sched\_yield() = status
  - Give up the CPU 
  - Probably seccomp
  
## mremap : remap a virtual address space
  mremap(old\_addr, old\_size, new\_size, flags) = pointer
  - The old address space is remapped to the new\_size and the pointer is returned
  - basically just realloc lol
  - probably seccomp
  
## msync : sync memory map to hardware
  msync(addr, length, flags) = status
  - probably seccomp
  
## mincore : return information about virtual memory of process
  mincore(addr, length, vector) = status
  - Check if virtual memory is in RAM
  - probably seccomp
  
## madvise : give advice about use of memory
  madvise(addr, length, advice) = status
  - Advise kernel about memory usage
  - probably seccomp
  
## shmget : allocates a system V shared memory segment
  shmget(key, size, shmflag) = ptr
  - Allocates shared memory segment
  - Not useful? having shared memory is almost useless
  - probably seccomp
  
## shmat : attach a shared memory segment to the process
  shmat(id, addr, flag) = addr
  - Attaches shared memory segments to process
  - Not useful
  - probably seccomp
  - IPC, so no
  
## shmctl : apply control operations to shared memory segment
  shmctl(id, cmd, buf) = output
  - Performs control operations on shared memory segment
  - Not useful
  - Shared memory segments should probably be disabled on OJ's
  - IPC is not really allowed on OJ's
  - seccomp

## dup : duplicate file descriptor
  dup(fd) = newfd
  - duplicates fd and returns an equivalent fd
  - harmless
  - seccomp
  
## dup2 : duplicate file descriptor, reusing the other fd
  dup(oldfd, newfd) = newfd
  - duplicates oldfd and returns newfd
  - if newfd is in use, it is closed and newfd refers to the same fd as newfd
  - harmless, seccomp
  
## pause : block until a signal is sent
  pause() = -1
  - probably fine, seccomp
  
## nanosleep
  nanosleep(time, time\_rem) = status
  - sleep high resolution
  - seccomp
  
## getitimer : get the value of an interval timer
  getitimer(kind\_of\_timer, current\_value) = status
  - send signal periodically after interval
  - not useful
  - seccomp
  
## alarm : send SIGALRM to be delivered in the future
  alarm(seconds) = prev\_seconds
  - returns the previous alarm seconds
  - not useful
  - seccomp
  
## setitimer : set the value of an interval timer
  setitimer(kind\_of\_timer, new\_val, old\_val) = status
  - like getitimer but set timer
  - not useful
  - seccomp
  
## getpid : get process id 
  getpid() = pid
  - returns current process id
  - seccomp
  
## sendfile : transfer data between files
  sendfile(write\_fd, read\_fd, offset, count) = count
  - copy count bytes from read file at offset to write file
  - not that helpful
  - seccomp
  
## socket : create socket for communication
  socket(domain, type, protocol) = fd
  - No
  - absolutely not
  - seccomp
  
## connect : connect socket to address
  connect(fd, addr, len) = status
  - No
  - would require socket fd
  - seccomp
  
## accept : accept connection on socket
  accept(fd, addr, len) = newfd
  - Would require socket fd
  - seccomp
  
## sendto : send message on socket
  sendto(fd, buf, len, flags) = cnt
  - requires a socket
  - No
  
## recvfrom : receive message on socket
  recvfrom(fd, buf, len, flags, addr, len) = cnt
  - requires a socket
  - No
  
## sendmsg : send message on socket
  sendmsg(fd, msg, flags) = cnt
  - requires a socket
  - Also sends control info
  - No

## recvmsg : recv message on socket
  recvmsg(fd, msg, flags) = cnt
  - requires a socket
  - No
  
## shutdown : shutdown socket
  shutdown(fd, how) = status
  - requires a socket
  - No
  
## bind : bind socket to address
  bind(fd, addr, addrlen) = status
  - requires a socket
  - No
  
## listen : listen for connections on socket
  listen(fd, backlog) = status
  - requires a socket
  - No
  
## getsockname : get address of socket
  getsockname(fd, addr, addrlen) = status
  - requires a socket
  - No
  
## getpeername : get address of peer on socket
  getpeername(fd, addr, addrlen) = status
  - requires a socket
  - No
  
## socketpair : make socket pair
  socketpair(domain, type, protocol, pair) = status
  - create socket pair
  - No
  
## setsockopt : set socket options
  setsockopt(fd, level, option, value, len) = status
  - Set socket option
  - requires a socket
  - No
  
## getsockopt : get socket option
  getsockopt(fd, level, option, val, len) = status
  - get socket option
  - requires a socket
  - no
  
## clone : make a new process / thread
  clone(flags, stack, parent\_tidptr, child\_tidptr, thread\_local\_storage) = pid
  - Makes a new process which can share numerous parts of it's memory with the process
  - We can limit threads with RLIMIT\_NPROC, but allowing subs to create processes seems like a poor choice
    - Mostly because of rlimits
  - Only allow CLONE\_THREAD?
  - Not seccomp
  
## fork : make a new process
  fork() = pid
  - equivalent to clone with flags = SIGCHLD
  - Makes a new process: no
  
## vfork : fork, but suspend execution of caller until callee releases its memory (with execve or similar)
  vfork() = pid
  - equivalent to clone with flags = CLONE\_VM | CLONE\_VFORK | SIGCHLD
  - Make a new process: no
  
## execve : execute a program
  execve(path, args, env) = ?
  - needs exactly one syscall
  - Only one
    - Much easier to check than filesystem check
    - harder to screw up
  - No reasonable program would use execve more than exactly once
  
## exit : exit the current thread
  exit(status) = ?
  - exit the thread
  - Required to exit programs
  - do not whitelist, use it to detect whether process spawned or not
  
## wait4 : wait for process to change
  wait4(pid, status, options, rusage) = pid
  - not useful, since there are no child processes
  
## kill : send signal to process
  kill(pid, sig) = status
  - only send to itself
  - otherwise, not safe
  
## uname : get info about kernel
  uname(info) = statuss
  - harmless
  
## semget : get System V semaphore
  semget(key, num, flags) = id
  - make a semaphore
  - basically a semaphore is a shared integer
  - IPC, so no...
  
## semop : semaphore operations
  semop(semid, options, num) = status
  - do operation on semaphore
  - no?
  
## semctl : semaphore control operations
  semctl(semid, semnum, cmd, semunion) = output
  - do control operation on semaphore
  - no
  
## shmdt : shared memory detachment
  shmdt(addr) = status
  - detach shared memory segment from shmat
  - IPC, no
  
## msgget : get message from system V message queue
  msgget(key, flag) = id
  - IPC, so no
  - message queues
  
## msgsnd : send message
  msgsnd(id, msg, size, flags) = status
  - IPC, so no
  
## msgrcv : receive message
  msgrcv(id, msg, size, type, flags) = count
  - IPC
  
## msgctl : message queue control operations
  msgctl(id, cmd, buf) = output
  - IPC
  
## fcntl : manipulate file descriptor
  fcntl(fd, cmd, arg) = status
  - Harmless, they cannot change access modes
  
## flock : operations on file lock
  flock(fd, operation) = status
  - locks file descriptor
  - Probably harmless
  - usually used in fcntl
  
## fsync : sync data to hardware
  fsync(fd) = status
  - flushes to hardware
  - Not a problem
  
## fdatasync : sync data, but not metadata unless necessary
  fdatasync(fd) = status
  - same as fsync
  
## truncate : truncate file to length
  truncate(path) = status
  - Should never be used
  
## ftruncate : same as truncate but with fd
  ftruncate(fd) = status
  
## getdents : get directory entries
  getdents(fd, elements, count) = status
  - get directory entries from fd
  - only valid if they have an fd

## getcwd : get current working directory
  getcwd(buf, size) = ptr
  - get CWD
  - harmless
  
## chdir : change CWD
  chdir(path) = status
  - change CWD, probably not useful
  - could check path?
  - probably just deny
  
## fchdir : change CWD with fd
  fchdir(fd) = status
  - same as chdir, but with fd, so allow if we allow chdir
  
## rename : rename file
  rename(oldpath, newpath) = status
  - rename file
  - absolutely not
  
## mkdir : make directory
  mkdir(path, mode) = status
  - make directory 
  - no
  
## rmdir : remove directory
  rmdir(path) = status
  - remove directory
  - **must be empty**
  - no
  
## creat : create file
  creat(path, mode) = fd
  - create a file
  - no
  - maybe for compilers?
  
## link : create hard link
  link(oldpath, newpath) = status
  - hard link newpath to refer to oldpath
  - no
  
## unlink : remove a link to a file
  unlink(path) = status
  - no
  
## symlink : make symbolic link to a file
  symlink(target, linked\_path) = status
  - no
  
## readlink : read value of symlink
  readlink(pathname, buffer, size) = length
  - no
  
## chmod : change mode of file
  chmod(pathname, mode) = status
  - change mode of file
  - no
  
## fchmod : change mode of file
  fchmod(fd, mode) = status
  - change mode of file fd
  - no
  
## chown : change ownership of file
  chown(path, owner, group) = status
  - change owner of file
  - no
  
## fchown : change ownership of file
  fchown(fd, owner, group) = status
  - no
  
## lchown : change ownership of link
  lchown(path, owner, group) = status
  - no
  
## umask : set file mode mask of process
  umask(mask) = mask
  - no
  
## gettimeofday : obvious
  gettimeofday(time, timezone) = status
  - no problem
  
## getrlimit : get resource limit
  getrlimit(resource, rlimit) = status
  - get resource
  - just a get, no problem
  
## getrusage : get resource usage
  getrusage(who, usage) = status
  - GET, no problem
  
## times : get time elapsed for process
  times(time) = clock\_ticks
  - GET, probably fine
  - DMOJ doesnt whitelist it though
  - probably not a problem
  
## ptrace : trace a child process
  ptrace(request, pid, addr, data) = output
  - No
  - Never should this be used
 
## syslog : manage kernel logs?
  syslog(type, buf, len) = output
  - glibc doesnt even have a wrapper for this lol
  - not useful

## getgid : get group id
  getgid() = gid
  - No problem
  
## setuid : set user id
  setuid(uid) = status
  - No
  - Requires root perms anyway
  
## setgid : set group id
  setgid(gid) = status
  - No
  - Requires root perms anyway
  
## geteuid : get effective user id
  geteuid() = euid
  - No problem
  
## getegid : get effective group id
  getegid() = egid
  - No problem
  
## setpgid : set process group id
  setpgid(pid, pgid) = status
  - No
  
## getppid : get parent process id
  getppid() = ppid
  - No problem
  
## getpgrp : get process group id
  getpgrp() = status
  - No problem
  
## setsid : create a session and set the process group ID
  setsid() = pid
  - No
  
## setreuid : set real / effective uid
  setreuid(uid, euid) = status
  - No
  
## setregid : set real / effective gid
  setregid(gid, egid) = status
  - No
  
## getgroups : get group ids of process
  getgroups(size, list) = count
  - No problem
  
## setgroups : set group ids of process
  setgroups(size, list) = status
  - No
  
## setresuid : set real / effective / saved-set uid of process
  setresuid(uid, euid, suid) = status
  - No
  
## getresuid : get real / effective / saved-set uid of process
  getresuid(uid, euid, suid) = status
  - Fine
  
## setresgid : set real / effective / saved-set gid of process
  setresgid(gid, egid, sgid) = status
  - No

## getresgid : get real / effective / saved-set gid of process
  getresgid(gid, egid, sgid) = status
  - Fine
  
## getpgid : get process group ID of process
  getpgid(pid) = status
  - Fine
  
## setfsuid : set the uid used for filesystem checks
  setfsuid(fsuid) = pre-uid
  - No
  
## setfsgid : set the gid used for filesystem checks
  setfsgid(fsgid) = pre-gid
  - No
  
## getsid : get session id of process
  getsid(pid) = sid
  - Fine
  
## capget : get capabilities of threads
  capget(header, data) = status
  - Fine
  
## capset : set capabilities of threads
  capset(header, data) = status
  - No
  
## rt\_sigpending : get pending signals
  rt\_sigpending(set) = status
  - Fine
  - Not useful
  
## rt\_sigtimedwait : wait for and remove signal from pending signals
  rt\_sigtimedwait(set, info, timeout) = signal
  - Fine
  - Not useful
  
## rt\_sigqueueinfo : queue signal and data to send to process
  rt\_sigqueueinfo(pid, sig, info) = status
  - Similar to kill
  - So no
  
## rt\_sigsuspend : wait for signal
  rt\_sigsuspend(mask) = status
  - wait for signal
  - Not useful
  
## sigaltstack : set / get alternate stack for handler
  sigaltstack(stack, old\_stack) = status
  - required to register signal handlers
  - do they really need to?
  
## utime : modify last access & modification time of file
  utime(filename, times) = status
  - no
  
## mknod : make a filesystem node
  mknod(pathname, mode, type) = status
  - no
  
## uselib : use shared library
  uselib(library) = status
  - glibc has no wrapper for this
  - would require pathname checking

## personality : change personality of process
  personality(persona) = status
  - For compatability?
  - No?
  
## ustat : get filesystem stats
  ustat(device, stats) = status
  - No?
  
## statfs : get filesystem stats
  statfs(path, stats) = status
  - No?
  
## fstatfs : get filesystem stats
  fstatfs(fd, stats) = status
  - No
  
## sysfs : get filesystem type info
  sysfs(...) = output
  - Why?
  
## getpriority : get scheduling priority
  getpriority(which, who) = priority
  - Probably not worth it
  
## setpriority : set sched priority
  setpriority(which, who, prior) = status
  - No
  - Could be controlled with RLIMIT
  
## sched\_setparam : set sched parameter
  sched\_setparam(pid, param) = status
  - No
  
## sched\_getparam : get sched parameter
  sched\_getparam(pid, param) = status
  - Probably not
  
## sched\_setscheduler : set scheduling policy
  sched\_setscheduler(pid, policy, parameter) = status
  - No

## sched\_getscheduler : get scheduling policy
  sched\_getscheduler(pid) = policy
  - No problem
  - Probably not necessary?
  
## sched\_get\_priority\_max : get maximum priority for policy
  sched\_get\_priority\_max(policy) = priority
  - I guess?

## sched\_get\_priority\_min : get minimum priority for policy
  sched\_get\_priority\_min(policy) = priority
  - I guess?

## sched\_rr\_get\_interval : get SCHED\_RR interval for given process
  sched\_rr\_get\_interval(pid, timespec) = status
  - I guess?
  
## mlock : lock memory into RAM
  mlock(addr, len) = status
  - Probably not?
  
## munlock : unlock memory
  munlock(addr, len) = status
  - Probably not
  
## mlockall : lock all of VM into RAM
  mlockall(flags) = status
  - Probably not
  
## munlockall : unlock all of VM 
  munlockall() = status
  - Probably not
  
## vhangup : virtually hangup the terminal
  vhangup() = status
  - No
  - Requires perms anyway
  
## modify\_ldt : get or set a per-process LDT entry
  modify\_ldt(func, ptr, count) = output
  - Modify LDT entry
  - No, probably not good idea
  
## pivot\_root : change the root filesystem
  pivot\_root(new\_root, put\_old) = status
  - requires root perms
  - Absolutely not
  
## \_sysctl : change system params
  \_sysctl(args) = status
  - Absolutely not
  ```
  Glibc does not provide a wrapper for this system call; call it using
  syscall(2).  Or rather...  don't call it: use of this system call has
  long been discouraged, and it is so unloved that it is likely to
  disappear in a future kernel version.
  ```
  
## prctl : do process control operations
  prctl(option, ...) = output
  - Process control operations
  - Some are required?
  - option determines whether it's allowed or not
  - For now, probably just disable it

## arch\_prctl : architecture-specific process control operations
  arch\_prctl(code, addr) = status
  - for x86-64. sets value of FS and GS registers
  
## adjtimex : adjust kernel clock
  adjtimex(timestruct) = status
  - No
  
## setrlimit : set resource limits
  setrlimit(resource, limit) = status
  - No
  
## chroot : change root directory of process
  chroot(path) = status
  - Requires root anyway
  
## sync : sync all changes to cached file data to disk
  sync() = void
  - There should be no filesystem changes
  - this is a global operation
  - so probably not
  
## acct : switch process accounting on / off
  acct(pathname) = status
  - No
  
## settimeofday
  settimeofday(time, timezone) = status
  - No
  
## mount : mount filesystem
  mount(source, target, type, flags, data) = status
  - No
  - Requires root anyway
  
## umount2 : unmount filesystem
  umount(target, flags) = status
  - unmount filesystem
  
## swapon : start swapping RAM to device
  swapon(path, flags) = status
  - No
  
## swapoff : stop swapping RAM to device
  swapoff(path) = status
  - No
  
## reboot : change ctrl-alt-delete actions
  reboot(magic, magic, cmd, arg) = status
  - Absolutely not
  - Requires root perms anyway

## sethostname
  sethostname(str, len) = status
  - no
  
## setdomainname
  setdomainname(name, len) = status
  - no
  
## iopl : change I/O privilege level of calling process
  iopl(level) = status
  - no
  - root perms are needed anyway
  
## ioperm : change permissions of ports
  ioperm(from, num, perms) = status
  - no
  - requires root perms for raising perms
  
## create\_module : create a loadable module
  create\_module(name, size) = status
  - deprecated
  - requires perms
  - requires path resolution
  - no
  
## init\_module : load a module
  init\_module(module, len, params) = status
  - no
  - requires perms
  
## delete\_module : delete a module
  delete\_module(name, flags) = status
  - no
  - requires perms
  
## get\_kernel\_syms : get kernel symbols
  get\_kernel\_syms(table) = count
  - no?
  - probably harmless though
  
## query\_module : get info about module
  query\_module(name, which, buf, size, ret) = status
  - no
  - probably fine though
  
## quotactl : manipulate disk quotas
  quotactl(cmd, special, id, addr) = status
  - no
  - requires perms
  
## nfsservctl : interface with nfs daemon
  nfsservctl(cmd, arg, res) = status
  - deprecated
  - no
  
## getpmsg : not implemented

## putpmsg : not implemented

## afs\_syscall : NI

## tuxcall : NI

## security : NI

## gettid : get thread ID
  gettid() = tid
  - Yes
  
## readahead : read ahead so reads are from cache
  readahead(fd, offset, count) = status
  - sure
  
## setxattr : set extended attributes about inode
  setxattr(path, name, value, size, flags) = status
  - xattrs are name:value pairs about files/dirs
  - No
  - This is basically write perms
  
## lsetxattr : set xattr for link
  lsetxattr(path, name, value, size, flags) = status
  - no
  
## fsetxattr : set xattr for fd
  fsetxattr(fd, name, value, size, flags) = status
  - no
  
## getxattr : get xattr for file
  getxattr(path, name, val, size) = cnt
  - probably not
  - not a problem, **IFF WE CHECK PATH COMPONENT**
  
## lgetxattr : get xattr for link
  lgetxattr(path, name, val, size) = cnt
  - probably not
  - not a problem, **IFF WE CHECK PATH COMPONENT**
  
## fgetxattr : get xattr for fd
  fgetxattr(fd, name, val, size) = cnt
  - probably not
  - not a problem
  
## listxattr : list xattr pairs
  listxattr(path, list, size) = cnt
  - probably not
  - not a problem, **IFF WE CHECK PATH COMPONENT**
  
## llistxattr : list xattr pairs for link
  llistxattr(path, list, size) = cnt
  - probably not
  - not a problem, **IFF WE CHECK PATH COMPONENT**
  
## flistxattr : list xattr pairs for fd
  flistxattr(fd, list, size) = cnt
  - probably not
  - not a problem
  
## removexattr
  removexattr(path, name) = status
  - No
  
## lremovexattr
  removexattr(path, name) = status
  - No
  
## fremovexattr
  removexattr(fd, name) = status
  - No
  
## tkill : send signal to thread
  tkill(tid, signal) = status
  - deprecated
  - no
  
## time : get unix time
  time(val) = val
  - get unix time
  - sure?
  
## futex : fast user-space thread locking
  futex(addr, op, val, timeout, ...) = status
  - used by threading libs
  - fine
  
## sched\_setaffinity	: set CPU affinity mask
  sched\_setaffinity(pid, size, mask) = status
  - set the cpus which the process can run on
  - no
  
## sched\_getaffinity : get CPU affinity mask
  sched\_getaffinity(pid, size, mask) = status
  - probably hamrless
  - but no
  
## set\_thread\_area : set thread-local storage info
  set\_thread\_area(info) = status
  - probably not
  - should be fine
  
## io\_setup : setup asynchronous io context
  io\_setup(events, ret) = status
  - make an aio context
  - no?
  - but prob fine
  
## io\_destroy : destroy aio ctx
  io\_destroy(id) = status
  - no?
  - prob fine
  
## io\_getevents : read aio events
  io\_getevents(id, min, max, events, timeout) = cnt
  - no?
  - but prob fine
  
## io\_submit : submit aio event
  io\_submit(id, number, events) = cnt
  - see above
  
## io\_cancel : cancel aio event
  io\_cancel(id, event, result) = status
  - see above
  
## get\_thread\_area : get TLS info
  get\_thread\_area(info) = status
  - get info about TLS
  - no
  - probably fine
  
## lookup\_dcookie : get dir associated with "cookie"
  lookup\_dcookie(cookie, buf, len) = cnt
  - no
  - requires perms
  
## epoll\_create : create epoll instance
  epoll\_create(size) = fd
  - [epolls](http://man7.org/linux/man-pages/man7/epoll.7.html)
  - create epoll, used to poll fds, similar to poll()
  - no
  - probably fine
  
## epoll\_ctl\_old
  - Old version of syscall?
  - Make same verdict as epoll\_ctl
  
## epoll\_wait\_old : see above

## remap\_file\_pages : make mmaped file pages non-linear
  - why
  - deprecated
  - no
  
## getdents64 : see getdents()

## set\_tid\_address : set the address that is used for futex\_wake if clone uses CLONE\_CHILD\_CLEARTID
  set\_tid\_address(ptr) = tid
  - used by threading libs
  
## restart\_syscall : restart stopped syscall
  restart\_syscall()
  - used internally
  - probably required
  - even if not required, is not a problem
  
## semtimedop : see semop
  - same as semop, but has timing
  - semaphores are shared memory mangement things
  
## fadvise64 : advise kernel about file access patterns
  fadvise64(fd, offset, len, advice) = status
  - give advice about file accesses
  - sure
  
## timer\_create : create interval timer
  timer\_create(id, event, res) = status
  - probably not?
  - but it would probably be fine
  
## timer\_settime : set the time of an interval timer
  timer\_settime(timer, flags, new, old) = status
  - change interval timer
  - same as all interval timers
  
## timer\_gettime
  timer\_gettime(timer, current) = status
  - see above
  
## timer\_getoverrun : get overrun count for timer
  timer\_getoverrun(timer) = overrun
  - see above
  
## timer\_delete
  timer\_delete(timer) = status
  - see above
  
## clock\_gettime
  clock\_gettime(clk, time) = status
  - get is fine
  - but why?
  
## clock\_settime
  clock\_settime(clk, time) = status
  - set requires perms
  - no
  
## clock\_getres :  get resolution/precision of target clock
  clock\_getres(clk, res) = status
  - fine, but why
  
## clock\_nanosleep : sleep but choose the clock
  clock\_nanosleep(clock, time) = status
  - like nanosleep()
  
## exit\_group : exit all threads in a process
  exit\_group(status) = ?
  - same as exit()
  
## epoll\_wait : wait for epoll event
  epoll\_wait(fd, events, maxevents, timeout) = cnt
  - as for epoll\_create
  
## epoll\_ctl : operations on epoll
  epoll\_ctl(epfd, fd, op, event) = status
  - as for epolls
  
## tgkill : send signal to thread
  tgkill(pid, tid, sig) = status
  - as for kill
  
## utimes : update modification and access time
  utimes(filename, times) = status
  - as for utime
  
## vserver : NI

## mbind : set memory policy for range
  mbind(addr, len, mode, nodemask, maxnode, flags) = status
  - im not really sure of the uses of this
  - probably fine, similar to mprotect?
  - but no
  
## set\_mempolicy
  set\_mempolicy(addr, len, mode, nodemask, maxnode) = status
  - seems like it controls how memory allocation works for those nodes?
  - see above
  
## get\_mempolicy
  get\_mempolicy(res, mask, maxnode, addr, flags) = status
  - see above
  
## mq\_open : open message queue
  mq\_open(name, flags, mode, attr) = mqd
  - Message queues are IPC
  - so no
  - but would be fine
  
## mq\_unlink
  mq\_unlink(name) = status
  - see above
  
## mq\_send
  mq\_timedsend(mqd, msg, len, prior, timeout) = status
  - see above
  
## mq\_timedreceive
  mq\_timedreceive(mqd, msg, len, prior, timeout) = count
  - see above
  
## mq\_notify : get notified when message is available
  mq\_notify(mqd, event) = status
  - see above
  
## mq\_getsetattr
  mq\_getsetattr(mqd, newattr, oldattr) = status
  - man page says "do not use this syscall"
  - its the syscall used to implement the lib funcs mq\_getattr & mq\_setattr
  - see above
  
## kexec\_load : load kernel for later execution
  kexec\_load(entry, size, segments, flags) = status
  - requires perms
  - no
  
## waitid : wait, but can wait for different things (processes, pgrps)
  waitid(idtype, id, siginfo, options, rusage) = status
  - as for waitw
  
## add\_key : add or update system key
  add\_key(type, desc, payload, len, keyring) = key
  - why
  - no
  
## request\_key : get key
  request\_key(type, desc, callout, keyring) = key
  - see above
  
## keyctl
  keyctl(operation, ...) = output
  - see above
  
## ioprio\_set : set I/O sched priority
  ioprio\_set(which, who, prio) = status
  - why
  - no
  
## ioprio\_get
  ioprio\_get(which, who) = prio
  - see above
  
## inotify\_init : initialize object to monitor fds
  inotify\_init() = fd
  - why
  - similar to epoll?
  - no but fine
  
## inotify\_add\_watch : add fd to inotify watchlist 
  inotify\_add\_watch(fd, path, mask) = watch\_fd
  - see above
  - would require path validation

## inotify\_rm\_watch
  inotify\_rm\_watch(fd, wd) = status
  - see above
  
## migrate\_pages : move pages in process to another node
  migrate\_pages(pid, maxnode, oldnodes, newnodes) = cnt
  - why
  - no
  
## openat
  openat(dir, pathname, flags) = fd
  - see open()
  - requires path joining of dir + pathname
  
## mkdirat
  mkdirat(dir, path, mode) = status
  - as for mkdir
  
## mknodat
  mknodat(dir, path, mode, dev) = status
  - as for mknod
  
## fchownat
  fchownat(dir, path, owner, group, flags) = status
  - as for chown()
  
## futimesat
  futimesat(dir, path, times) = status
  - as for utimes
  
## newfstatat
  newfstatat(dir, file, stats, flag) = status
  - as for stat
  
## unlinkat
  unlinkat(dir, path, flags) = status
  - as for unlink
  
## renameat
  renameat(olddir, oldpath, newdir, newpath) = status
  - as for rename
  
## linkat
  linkat(olddir, oldpath, newdir, newpath, flags) = status
  - as for link
  
## symlinkat
  symlinkat(target, newdirfd, linkpath) = status
  - as for symlink
  
## readlinkat
  readlinkat(dir, path, buf, size) = len
  - as for readlink
  
## fchmodat
  fchmodat(dir, path, mode, flags) = status
  - as for chmod
  
## faccessat
  faccessat(dir, path, mode, flags) = status
  - as for access
  
## pselect6
  pselect6(num, read, write, except, timeout, sig) = status
  - as for select
  
## ppoll
  ppoll(fds, count, timeout, sig) = status
  - as for poll
  
## unshare
  unshare(flags) = status
  - unshare parts of process with other processes
  - probably not?
  - probably fine
  - probably can't be used in order to get past rlimits
  
## set\_robust\_list : set the head of the per-thread robust futex lists
  set\_robust\_list(head, len) = status
  - similar to futex()
  - probably fine
  - not used AFAIK
  
## get\_robust\_list
  get\_robust\_list(pid, head, len) = status
  - see above
  
## splice : move data to/from a pipe
  splice(in, offset\_in, out, offset\_out, len, flags) = cnt
  - move data without user space copy
  - why but should be fine
  
## tee : copy data without consuming
  tee(in, out, len, flags) = cnt
  - see above
  
## sync\_file\_range : sync file segment with disk
  sync\_file\_range(fd, offset, len, flags) = status
  - sync file, but for a range
  - as for fsync
  
## vmsplice : copy data to/from pipe
  vmsplice(fd, {addr, len}, segments, flags) = cnt
  - like splice, but copy from virtual memory
  
## move\_pages : move pages of a process to a different node
  move\_pages(pid, count, pages, nodes, status, flags) = status
  - another NUMA thing
  - why
  - should be fine, would require perms to move to another process
  
## utimensat : utimesat but with nanosecond precision
  futimesat(dir, path, times)

## epoll\_pwait : epoll\_wait but also wait for signal
  epoll\_pwait(fd, events, maxevents, timeout, sig) = cnt
  
## signalfd : create fd that can monitor signals
  signalfd(fd, mask) = newfd
  - probably fine but why
  
## timerfd\_create : create timer that has an fd
  timerfd\_create(clock, flags) = fd
  - analogous to timer\_create but returns fd
  
## eventfd : create fd for monitoring events
  eventfd(initval) = fd
  - why
  
## fallocate : directly allocate the file space
  fallocate(fd, mode, offset, len) = status
  - why
  
## timerfd\_settime
  timerfd\_settime(fd, flags, newval, oldval) = status
  
## timerfd\_gettime
  timerfd\_gettime(fd, res) = status
  
## accept4
  accept4(fd, addr, addrlen, flags) = sockfd
  - no
  
## signalfd4
  signalfd4(fd, mask, flags) = status
  - as for signalfd
  
## eventfd2
  eventfd2(initval, flags) = fd
  - as for eventfd
  
## epoll\_create1
  epoll\_create1(flags) = fd
  - as for epoll\_create
  
## dup3
  dup3(oldfd, newfd, flags) = status
  - as for dup2
  
## pipe2
  pipe2(fds, flags) = status
  - as for pipe
  
## inotify\_init1
  inotify\_init1(flags) = fd
  - as for inotify\_init but flags
  
## preadv : readv but with offset
  preadv(fd, buffers, len, offset) = cnt
  
## pwritev
  pwritev(fd, buffers, len, offset) = cnt
  - see above
  
## rt\_tgsigqueueinfo	: queue signal and info to thread
  rt\_tgsigqueueinfo(tgid, tid, sig, info) = status
  - why
  
## perf\_event\_open : monitor performance
  perf\_event\_open(attr, pid, cpu, group, flags) = fd
  - why
  
## recvmmsg : receive multiple messages with timeout
  recvmmsg(fd, msgs, len, flags, timeout) = cnt
  - for sockets, so no
  
## fanotify : monitor filesystem events
  fanotify\_init(flags, event\_flags) = fd
  - why
  
## fanotify\_mark : add, remove or modify fanotify mark
  fanotify\_mark(fd, flags, mask, dir, path) = status
  - see above
  
## prlimit64 : set limit for any process
  prlimit64(pid, resource, new, old) = status
  - validate pid
  
## name\_to\_handle\_at : get handle for file
  name\_to\_handle\_at(dir, path, handle, mountid, flags) = status
  - why 
  
## open\_by\_handle\_at : open by handle
  open\_by\_handle\_at(mount\_fd, handle, flags) = fd
  - why
  
## clock\_adjtime : ??

## syncfs : sync filesystem which fd resides in
  syncfs(fd) = status
  - why
  
## sendmmsg : send many messages
  sendmmsg(fd, msgs, len, flags) = cnt
  - no
  
## setns : set calling thread's namespace
  setns(fd, nstype) = status
  - no?
  
## getcpu : get cpu and numa node for process
  getcpu(cpu, node, cache) = status
  - why
  
## process\_vm\_readv : transfer data between processes
  process\_vm\_readv(pid, localdata, localcnt, remotedata, remotecnt, flags) = cnt
  - IPC
  - no
  
## process\_vm\_writev
  process\_vm\_writev(pid, localdata, localcnt, remotedata, remotecnt, flags) = cnt
  - no
  
## kcmp : check if processes share a resource
  kcmp(pid1, pid2, type, idx1, idx2) = cmp
  - why
  
## finit\_module : load kernel module
  finit\_module(fd, params, flags) = status
  - why
  - requires perms