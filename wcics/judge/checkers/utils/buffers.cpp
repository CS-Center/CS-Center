#include <unistd.h>

#include "buffers.hpp"

char _buf1[BUF_SIZE + 1];
char _buf2[BUF_SIZE + 1];

char* buf1 = _buf1;
char* buf2 = _buf2;

buffered_file::buffered_file(int fd, char* buf) : fd(fd), buf(buf), pos(0), _eof(false) { buf[0] = 0; }

int buffered_file::_do_read() {
  if(_eof || buf[pos]) return 0;

  int cnt = read(fd, buf, BUF_SIZE);
  
  if(cnt < 0) return -1;
  
  if(cnt == 0) {
    _eof = 1;
  }
  
  buf[cnt] = 0;
  
  return 0;
}

int buffered_file::get() {
  if(_do_read()) return FAIL;
  
  if(_eof) return 0;
  
  return buf[pos++];
}

bool buffered_file::eof() const {
  return _eof;
}