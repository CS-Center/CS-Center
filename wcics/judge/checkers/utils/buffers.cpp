#include <unistd.h>

#include "buffers.hpp"

#include "utils/debug.hpp"

char _buf1[BUF_SIZE + 1];
char _buf2[BUF_SIZE + 1];

char* buf1 = _buf1;
char* buf2 = _buf2;

buffered_file::buffered_file(int fd, char* buf) : fd(fd), buf(buf), pos(0), cur_cnt(0), _eof(false) { buf[0] = 0; }

void buffered_file::_do_read() {
  if(pos < cur_cnt || _eof) return;

  int cnt = RUNTIME_FUNC(read(fd, buf, BUF_SIZE));
  
  if(cnt == 0)
    _eof = true;
  
  cur_cnt = cnt;
  
  pos = 0;
  
  buf[cnt] = 0;
}

char buffered_file::get() {
  PUSH_STACK(_do_read());
  
  if(_eof) return 0;
    
  return buf[pos++];
}

bool buffered_file::eof() const {
  return _eof;
}