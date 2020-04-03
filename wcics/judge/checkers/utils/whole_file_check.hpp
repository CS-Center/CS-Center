typedef int (*whole_check_sig) (char* judge_out, char* user_out, const void* arg);

int whole_file_check(int jfd, int ufd, whole_check_sig func, const void* arg);