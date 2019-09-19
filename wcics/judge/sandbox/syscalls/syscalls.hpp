#include "inspectors.hpp"
#include "syscall_names.hpp"

#define HANDLER_DENY 0
#define HANDLER_INSPECT 1
#define HANDLER_ALLOW 2
#define HANDLER_WHITELIST 3
#define HANDLER_ERRNO 4
#define HANDLER_SPECIAL 5

// required for default_seccomp_filter to initialize properly
short* get_syscall_handlers();

// defined in man pages
typedef void* scmp_filter_ctx;

// the default seccomp filter to be loaded at child initialization time. 
// This can be changed in the child if required, although it is discouraged

extern scmp_filter_ctx default_seccomp_filter;

extern short* syscall_handlers;
extern inspector_sig* syscall_inspectors;