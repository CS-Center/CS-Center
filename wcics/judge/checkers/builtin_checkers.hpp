typedef int (*checker_sig) (int judgefd, int userfd, const char* arg);

checker_sig get_checker(const char* name);