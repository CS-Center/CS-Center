#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <fstream>
using namespace std;

#include "utils/files.hpp"
#include "utils/debug.hpp"

int file_len(int fd) {
	struct stat buf;

	RUNTIME_FUNC(fstat(fd, &buf));

	return buf.st_size;	
}

void write_to_file(string name, string content) {
  FILE* f = RUNTIME_FUNC_EQ(fopen(name, "wb"), 0);
  
  RUNTIME_FUNC_NEQ(fwrite(content.c_str(), content.size(), 1, f), content.size());
  
  RUNTIME_FUNC_NEQ(fclose(f), 0);
}

void write_to_file(int fd, const char* content, int len) {
  FILE* f = RUNTIME_FUNC_EQ(fdopen(fd, "wb"), 0);
  
  RUNTIME_FUNC_NEQ(fwrite(content.c_str(), content.size(), 1, f), content.size());
  
  RUNTIME_FUNC_NEQ(fclose(f), 0);
}

string read_from_file(string name) {
  FILE* f = RUNTIME_FUNC_EQ(fopen(name.c_str(), "rb"), 0);
  
  return _read_from_file(f);
}

string read_from_file(int fd) {
  FILE* f = RUNTIME_FUNC_EQ(fdopen(fd, "rb"), 0);
  
  return _read_from_file(f);
}

string _read_from_file(FILE* f) {
  RUNTIME_FUNC(fseek(f, 0, SEEK_END));
  
  int sz = RUNTIME_FUNC(ftell(f));
  
  string s(sz, '\0');
  
  RUNTIME_FUNC_NEQ(fread(&s[0], sz, 1, f), sz);
  
  return s;
}