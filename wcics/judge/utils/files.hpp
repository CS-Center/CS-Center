#include <string>

int file_len(int fd);

void write_to_file(const char* name, std::string content);
void write_to_file(int fd, std::string content);

std::string read_from_file(std::string name);
std::string read_from_file(int fd);