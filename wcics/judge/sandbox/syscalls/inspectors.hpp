#pragma once

#include "sandbox/process/secure_process/secure_process.hpp"

typedef int (*inspector_sig) (SecureProcess&);

int exec_check (SecureProcess&);

int empty_check(SecureProcess&);
int stat_check(SecureProcess&);
int fstatat_check(SecureProcess&);
int open_check(SecureProcess&);
int openat_check(SecureProcess&);

int clone_check (SecureProcess&);

int self_check(SecureProcess&);

int prlimit_check(SecureProcess&);

int prctl_check(SecureProcess&);