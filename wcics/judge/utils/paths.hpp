#pragma once

#include <limits.h>

// returns 0 on success, -1 on error
// EINVAL : path was not absolute
// ENAMETOOLONG : path was too long
int normalize_abs(const char* path, char buf[PATH_MAX]);

// 0 on sucess, -1 _or_ -2 on error
// throws ENAMETOOLONG if file is relative and dir + file is too long
  // in this case it returns -2 to distinguish from ENAMETOOLONG thrown by normalize_abs
// also can throw anything thrown by normalize_abs
int fulljoin(const char* dir, const char* file, char buf[PATH_MAX]);
