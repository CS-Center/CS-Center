#include <string>

int file_len(int fd);

int get_temp_fd();

void write_to_file(const char* name, std::string content);
void write_to_file(int fd, std::string content);

std::string read_from_file(const char* name);
std::string read_from_file(int fd);