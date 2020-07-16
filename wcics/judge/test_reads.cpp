#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
using namespace std;

const char* filename = "dumpfile.txt";

const int blen = 1 << 28;

const int sblen = 1 << 12;

char buf[sblen];

char fullbuf[blen];

int main(int argc, const char** argv) {
	if(argc > 1 && argv[1][0] == '1') {
		FILE* f = fopen(filename, "r");
    
    for(int x = 0; x < blen; x++) {
      if(argv[1][0] == '2') 
        char c = getc_unlocked(f);
        
		  else 
        char c = getc_unlocked(f);
    }

		fclose(f);
	}
  else if(argc > 1 && argv[1][0] == '2') {
    int fd = open(filename, O_RDONLY | O_CLOEXEC);
		
		if(fd < 0) {
			perror("open");
			return 0;
		}
    
    int cnt = 0, offset = 0;
    
    do {      
      offset += cnt;
          
      cnt = read(fd, fullbuf, blen - offset);

    } while(cnt > 0);
    
    for(int x = 0; x < blen; x++) {
      char c = fullbuf[x];
    }
    
    if(cnt < 0) perror("read");
    if(close(fd) < 0) perror("close");
  } 
  
  else if(argc > 1 && argv[1][0] == '3') {
    FILE* f = fopen(filename, "r");
    
    fread(fullbuf, blen, 1, f);
    
    fclose(f);
  }
  
	else {
		int fd = open(filename, O_RDONLY | O_CLOEXEC);
		
		if(fd < 0) {
			perror("open");
			return 0;
		}
    
    int cur_cnt = 0, cur_max_cnt = 0;
    
    for(int x = 0; x < blen; x++) {
      if(cur_cnt == 0) {
        int r = read(fd, buf, sblen);
        
        cur_cnt = cur_max_cnt = r;
        
        if(r < 0) perror("read");
      }
      
      char c = buf[cur_max_cnt - cur_cnt];
      
      cur_cnt--;
    }

		if(close(fd) < 0) perror("close");
	}
}
