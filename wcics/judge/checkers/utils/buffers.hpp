#define BUF_SIZE 262144

// These buffers are used by the checkers

// BUF_SIZE + 1 chars
extern char* buf1;
extern char* buf2;

// buf should be one of these buffers or another of BUF_SIZE + 1 length
class buffered_file {
private:
  int _do_read();
  
  bool _eof;
  
  int pos, cur_cnt;
  
  char* buf;
  
public:
  const int fd;

  buffered_file(int fd, char* buf);
      
  int get(); // return and pop
  
  bool eof() const;
};