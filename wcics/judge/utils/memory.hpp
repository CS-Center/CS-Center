#pragma once

#include <sys/types.h>

long get_proc_memory_field(pid_t p, const char* pat);

inline long get_max_rss(pid_t p) {
  return get_proc_memory_field(p, "VmHWM");
}