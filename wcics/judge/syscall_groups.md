# Syscall Groups

## Read Syscalls : Whitelist
read
pread64
readv
preadv

## Write Syscalls : Whitelist
write
pwrite64
writev
pwritev

## File Access : Inspect
open
stat
access
readlink
openat
newfstatat
readlinkat
faccessat

## Get xattrs : Inspect but deny
getxattr
lgetxattr
listxattr
llistxattr

## fget xattrs : safe but deny
fgetxattr
flistxattr

## File manipulation on descriptors : Whitelist
close
fstat
lstat
poll
lseek
select
dup
dup2
sendfile
fcntl
flock
fsync
fdatasync
getdents
readahead
getdents64
fadvise64
pselect6
ppoll
sync\_file\_range
dup3

## File handles : Probably safe but deny
name\_to\_handle\_at
open\_by\_handle\_at

## Filesystem modifications : Deny
truncate
ftruncate
rename
mkdir
creat
link
unlink
symlink
chmod
fchmod
chown
fchown
lchown
utime
mknod
pivot\_root
\_sysctl
chroot
sync
acct
mount
umount2
swapon
swapoff
setxattr
lsetxattr
fsetxattr
removexattr
lremovexattr
fremovexattr
utimes
mkdirat
mknodat
fchownat
unlinkat
renameat
linkat
symlinkat
fchmodat
utimensat
syncfs

## Filesystem stats : deny
ustat
statfs
fstatfs
sysfs
fanotify\_init
fanotify\_mark

## Device control : deny
quotactl
ioctl

## Process modifications : Deny
chdir
fchdir
umask
setgid
setpgid
setsid
setreuid
setregid
setgroups
setresuid
setfsuid
setfsgid
capset
prctl

## Allowed Process modifications : Whitelist
arch\_prctl

## Memory management : Whitelist
mmap
mprotect
munmap
brk
mremap
msync
mincore
madvise

## Memory node policys : Deny, might be safe
mbind
set\_mempolicy
get\_mempolicy
migrate\_pages
move\_pages

## File memory allocation : deny. Probably not safe
fallocate

## Memory : deprecated deny
remap\_file\_pages

## Memory locking : Deny
mlock
munlock
mlockall
munlockall

## Signals : Allow
rt\_sigaction
rt\_sigprocmask
rt\_sigreturn
pause
rt\_sigpending
rt\_sigtimedwait
rt\_sigsuspend
sigaltstack

## Time

### Get : allow
gettimeofday
times
time
clock\_gettime
clock\_getres

### Set : deny
adjtimex
settimeofday
clock\_settime
clock\_adjtime

## IPC

### Pipes : deny, but may be required (?)
pipe
pipe2

#### pipe data movement : deny, but could be safe
splice
tee
vmsplice

### Shared Memory : deny
shmget
shmat
shmctl
shmdt

### Semaphores : deny
semget
semop
semctl
semtimedop

### Message queues : deny
msgget
msgsnd
msgrcv
msgctl
mq\_open
mq\_unlink
mq\_timedsend
mq\_timedsend
mq\_timedreceive
mq\_notify
mq\_getsetattr

### Transfer data : deny
process\_vm\_readv
process\_vm\_writev

## CPU Scheduling

### Get : whitelist
sched\_yield
getpriority
sched\_getparam
sched\_getscheduler
sched\_get\_priority\_max
sched\_get\_priority\_min
sched\_rr\_get\_interval
sched\_getaffinity

### Set : Deny
setpriority
sched\_setparam
sched\_setscheduler
sched\_setaffinity

## Sleep : allow
nanosleep
clock\_nanosleep

## Timers : deny but probably safe
getitimer
setitimer
timer\_create
timer\_settime
timer\_gettime
timer\_getoverrun
timer\_delete

## Alarm : whitelist
alarm

## Misc. Info : whitelist
getpid
uname
getcwd
getgid
geteuid
getegid
getppid
getpgrp
getgroups
getresuid
getresgid
getpgid
getsid
capget
gettid

## Rlimits

prlimit64 : inspect pid

### Get : whitelist
getrlimit
getrusage

### Set : deny
setrlimit

## Sockets : absolutely deny
socket
connect
accept
sendto
recvfrom
sendmsg
recvmsg
shutdown
bind
listen
getsockname
getpeername
socketpair
setsockopt
getsockopt
accept4
recvmmsg
sendmmsg

## New Process

### New thread
clone : inspect CLONE\_THREAD

### New process
fork
vfork

## Execve : inspect allow once
execve

## exit : allow
exit
exit\_group

## wait : deny
wait4
waitid

## Send signals : deny or inspect pid
kill
rt\_sigqueueinfo
tkill
tgkill
rt\_tgsigqueueinfo

## System commands : deny
syslog
reboot
sethostname
setdomainname
nfsservctl

## Kernel commands : deny
get\_kernel\_syms

## Modules : deny
create\_module
init\_module
delete\_module
query\_module
finit\_module

## Not implemented : deny
getpmsg
putpmsg
afs\_syscall
tuxcall
security
vserver

## Thread sync : whitelist
futex
set\_tid\_address

## Internals : whitelist
restart\_syscall

## Thread area : whitelist
set\_thread\_area
get\_thread\_area

## Async io : deny but probably safe
io\_setup
io\_destroy
io\_getevents
io\_submit
io\_cancel

## Epoll : deny but safe
epoll\_create
epoll\_ctl\_old
epoll\_wait\_old
epoll\_wait
epoll\_ctl
epoll\_pwait
epoll\_create1

## Signal fds : deny but safe
signalfd
signalfd4

## eventfd : deny but safe
eventfd
eventfd2

## timerfd : deny but safe
timerfd\_create
timerfd\_settime
timerfd\_gettime

## Keys : deny
add\_key
request\_key
keyctl

## IO priority : deny
ioprio\_get

## fd monitor object : deny but safe
inotify\_init
inotify\_add\_watch
inotify\_rm\_watch
inotify\_init1

## robust lists : deny
get\_robust\_list
set\_robust\_list

## Namespaces : deny
setns

## Other : deny
ptrace
uselib
personality
vhangup
modify\_ldt
iopl
lookup\_dcookie
kexec\_load
unshare
perf\_event\_open
kcmp