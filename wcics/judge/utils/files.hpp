#include <string>

int file_len(int fd);

void write_to_file(string name, string content);
void write_to_file(int fd, string content);

std::string read_from_file(string name);
std::string read_from_file(int fd);