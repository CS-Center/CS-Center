#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>

#include "sandbox/file-access-checker/file_access_checker.hpp"
#include "executors/clang++/clangpp_executor.hpp"
#include "sandbox/communicator.hpp"
#include "utils/pipes.hpp"
#include "executors/executor_info.hpp"

int main(int argc, char** argv) {
  for(ExecutorInfo info : executors)
    printf("%s: %s\n", info.fullname, info.get_info());
  //return 0;

	const char* dirs[] = {
		"/lib",
		"/usr",
    "/tmp/box",
    "/proc/self",
		0
	};

	const char* reads[] = {
    "/",
    "/etc",
    "/etc/ld.so.nohwcap",
    "/etc/ld.so.preload",
    "/etc/ld.so.cache",
    "/etc/passwd",
    "/etc/nsswitch.conf",
    "/dev/null",
    "/sys/devices/system/cpu",
    "/sys/devices/system/cpu/online",
    "/etc/localtime", 
    "/proc/meminfo",
    "/proc/cpuinfo",
    "/etc/os-release",
    "/proc/sys/vm/overcommit_memory",
    0
  };
  
	const char* writes[] = {
    "/dev/null"
  };
  
  int status;

	FileAccessChecker fac(dirs, reads, writes, status);
  
	config conf;
  conf.timelimit = 1;
  conf.dir = "/tmp/box";
  conf.memory = 32 * 1024 * 1024;
  
  char inbuf[1024] = "", outbuf[1 << 20], errbuf[1 << 20];
  
  communicate_fd in, out, err;
  
  out.length = err.length = 1 << 20;

  in.length = strlen(inbuf);
  
  in.buffer = inbuf;
  out.buffer = outbuf;
  err.buffer = errbuf;
    
  Communicator comm(in, out, err, conf, PIPE_NORMAL, PIPE_NORMAL, PIPE_NORMAL, status);
  
  AsyncCommunicator acomm(comm);
  
  SharedProcessResult res(status);

  const char* const null[] = {0};
	Clangpp11Executor c1e("#include<stdio.h>\nint main(){puts(\"Hello, World!\");}", "Submission", null, null, conf, acomm, fac, res);
  
  status = c1e.prepare();
  
  if(status) {
    puts("IE when preparing executor");
    
    puts((*res).info);
    
    return -1;
  }
  
  c1e.launch();
  
  c1e.cleanup();
  
  printf("Time elapsed: %f\n", (*res).time);
  
  printf("RSS: %ld bytes\n", (*res).rss);
  
  printf("res.info: '%s'\n", (*res).info);
  
  printf("stdout: '%s'\n", out.buffer);
  
  printf("stderr: '%s'\n", err.buffer);
}
