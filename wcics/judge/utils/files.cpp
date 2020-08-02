#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#include "utils/files.hpp"
#include "utils/debug.hpp"

int file_len(int fd) {
	struct stat buf;

	RUNTIME_FUNC(fstat(fd, &buf));

	return buf.st_size;
}

int get_temp_fd() {
	return RUNTIME_FUNC(open("/tmp", O_TMPFILE | O_CLOEXEC | O_RDWR, 0600));
}

void write_to_file(const char* name, std::string content) {
  FILE* f = RUNTIME_FUNC_EQ(fopen(name, "wb"), (FILE*)0);

  RUNTIME_FUNC_NEQ(fwrite(content.c_str(), 1, content.size(), f), content.size());

  RUNTIME_FUNC_NEQ(fclose(f), 0);
}

void write_to_file(int fd, std::string content) {
  FILE* f = RUNTIME_FUNC_EQ(fdopen(fd, "wb"), (FILE*)0);

  RUNTIME_FUNC_NEQ(fwrite(content.c_str(), 1, content.size(), f), content.size());
}

std::string _read_from_file(FILE* f) {
  RUNTIME_FUNC(fseek(f, 0, SEEK_END));

  int sz = RUNTIME_FUNC(ftell(f));

	RUNTIME_FUNC(fseek(f, 0, SEEK_SET));

  std::string s(sz, '\0');

	size_t cnt = fread(&s[0], 1, sz, f);

  RUNTIME_FUNC_NEQ(cnt, (size_t)sz);

  return s;
}

std::string read_from_file(const char* name) {
  FILE* f = RUNTIME_FUNC_EQ(fopen(name, "rb"), (FILE*)0);

  std::string ret = _read_from_file(f);

  RUNTIME_FUNC_NEQ(fclose(f), 0);

  return ret;
}

std::string read_from_file(int fd) {
  FILE* f = RUNTIME_FUNC_EQ(fdopen(fd, "rb"), (FILE*)0);

  return _read_from_file(f);
}
