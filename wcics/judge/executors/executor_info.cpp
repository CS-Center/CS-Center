#include <string.h>
#include <string>
#include <fcntl.h>
#include <vector>

#include "executors/clang_executor.hpp"
#include "executors/clangpp_executor.hpp"
#include "executors/cpython_executor.hpp"
#include "executors/gpp_executor.hpp"
#include "executors/gcc_executor.hpp"
#include "executors/java_executor.hpp"
#include "executors/pypy_executor.hpp"

#include "sandbox/process/insecure_process.hpp"

#include "utils/pipes.hpp"
#include "utils/files.hpp"
#include "utils/debug.hpp"

#include "executor_info.hpp"

using namespace std;

#define ADD_INFO(name, args...) res.emplace_back(EXEC, args, VFLAG, RUNTM, LANG, [](EXEC_INIT_ARGS){return new name(EXEC_INIT_ARG_NAMES); });

std::vector<ExecutorInfo> get_executors() {
  std::vector<ExecutorInfo> res;
	
	#define VFLAG "--version"
	
	#define LANG "C"
			
		#define EXEC CLANG_PATH
		#define RUNTM "Clang"
	
			#if HAS_GCC_90 
				ADD_INFO(ClangExecutor<90>, "Clang 90", "clang90", 90, false);
			#endif

			#if HAS_GCC_99
				ADD_INFO(ClangExecutor<99>, "Clang 99", "clang99", 99, false);
			#endif

			#if HAS_GCC_11
				ADD_INFO(ClangExecutor<11>, "Clang 11", "clang11", 11, false);
			#endif
	
		#undef EXEC
		#undef RUNTM
	
		#define EXEC GCC_PATH
		#define RUNTM "GCC"

			#if HAS_GCC_90
				ADD_INFO(GCCExecutor<90>, "GCC 90", "gcc90", 90, false);
			#endif
			
			#if HAS_GCC_99
				ADD_INFO(GCCExecutor<99>, "GCC 99", "gcc99", 99, false);
			#endif
	
			#if HAS_GCC_11
				ADD_INFO(GCCExecutor<11>, "GCC 11", "gcc11", 11, false);
			#endif
	
		#undef EXEC
		#undef RUNTM
	#undef LANG
	
	#define LANG "C++"
	
		#define EXEC CLANGPP_PATH
		#define RUNTM "Clang++"
		
			#if HAS_CLANGPP_98
				ADD_INFO(ClangppExecutor<98>, "Clang++ 98", "clang++98", 98, false);
			#endif
			
			#if HAS_CLANGPP_98
				ADD_INFO(ClangppExecutor<98>, "Clang++ 98", "clang++98", 98, false);
			#endif
			
			#if HAS_CLANGPP_98
				ADD_INFO(ClangppExecutor<98>, "Clang++ 98", "clang++98", 98, false);
			#endif
			
			#if HAS_CLANGPP_98
				ADD_INFO(ClangppExecutor<98>, "Clang++ 98", "clang++98", 98, false);
			#endif
			
			#if HAS_CLANGPP_98
				ADD_INFO(ClangppExecutor<98>, "Clang++ 98", "clang++98", 98, false);
			#endif
			
			
	
	#undef LANG

  #ifdef CPP_ENABLED

    #ifdef CLANGPP_ENABLED
      const char* clangpp_args[] = {"clang++", "-dumpversion", 0};

      #ifdef CLANGPP_98_ENABLED
        ADD_INFO(Clangpp98, CLANGPP_PATH, "Clang++98", "clang++98", "C++", 98, "Clang++", clangpp_args, 0);
      #endif

      #ifdef CLANGPP_03_ENABLED
        ADD_INFO(Clangpp03, CLANGPP_PATH, "Clang++03", "clang++03", "C++", 3, "Clang++", clangpp_args, 0);
      #endif

      #ifdef CLANGPP_11_ENABLED
        ADD_INFO(Clangpp11, CLANGPP_PATH, "Clang++11", "clang++11", "C++", 11, "Clang++", clangpp_args, 0);
      #endif

      #ifdef CLANGPP_14_ENABLED
        ADD_INFO(Clangpp14, CLANGPP_PATH, "Clang++14", "clang++14", "C++", 14, "Clang++", clangpp_args, 0);
      #endif

      #ifdef CLANGPP_17_ENABLED
        ADD_INFO(Clangpp17, CLANGPP_PATH, "Clang++17", "clang++17", "C++", 17, "Clang++", clangpp_args, 0);
      #endif

    #endif

    #ifdef GPP_ENABLED
      const char* gpp_args[] = {"g++", "-dumpversion", 0};

      #ifdef GPP_98_ENABLED
        ADD_INFO(Gpp98, GPP_PATH, "G++98", "g++98", "C++", 98, "G++", gpp_args, 0);
      #endif

      #ifdef GPP_03_ENABLED
        ADD_INFO(Gpp03, GPP_PATH, "G++03", "g++03", "C++", 3, "G++", gpp_args, 0);
      #endif

      #ifdef GPP_11_ENABLED
        ADD_INFO(Gpp11, GPP_PATH, "G++11", "g++11", "C++", 11, "G++", gpp_args, 0);
      #endif

      #ifdef GPP_14_ENABLED
        ADD_INFO(Gpp14, GPP_PATH, "G++14", "g++14", "C++", 14, "G++", gpp_args, 0);
      #endif

      #ifdef GPP_17_ENABLED
        ADD_INFO(Gpp17, GPP_PATH, "G++17", "g++17", "C++", 17, "G++", gpp_args, 0);
      #endif

    #endif

  #endif

  #ifdef JAVA_ENABLED

    const char* java_args[] = {"java", "-version", 0};

    #ifdef JAVA_8_ENABLED
      ADD_INFO(Java8, JAVA_VM_PATH, "Java8", "java8", "Java", 8, "OpenJDK", java_args, 1);
    #endif

    #ifdef JAVA_11_ENABLED
      ADD_INFO(Java11, JAVA_VM_PATH, "Java11", "java11", "Java", 11, "OpenJDK", java_args, 1);
    #endif

  #endif

  #ifdef PYTHON_ENABLED

    #ifdef CPYTHON_ENABLED

      #ifdef CPYTHON2_ENABLED
        const char* cpython2_args[] = {"python2", "-V", 0};
        
        ADD_INFO(CPython2, CPYTHON2_PATH, "CPython2", "cpython2", "Python", 2, "CPython", cpython2_args, 1);
      #endif

      #ifdef CPYTHON3_ENABLED
        const char* cpython3_args[] = {"python3", "-V", 0};
        
        ADD_INFO(CPython3, CPYTHON3_PATH, "CPython3", "cpython3", "Python", 3, "CPython", cpython3_args, 0);
      #endif

    #endif

    #ifdef PYPY_ENABLED

      #ifdef PYPY2_ENABLED
        const char* pypy2_args[] = {"pypy2", "-V", 0};
      
        ADD_INFO(PyPy2, PYPY2_PATH, "PyPy2", "pypy2", "Python", 2, "PyPy", pypy2_args, 1);
      #endif

      #ifdef PYPY3_ENABLED
        const char* pypy3_args[] = {"pypy3", "-V", 0};
      
        ADD_INFO(PyPy3, PYPY3_PATH, "PyPy3", "pypy3", "Python", 3, "PyPy", pypy3_args, 0);
      #endif

    #endif

  #endif
  
  return res;
}

std::vector<ExecutorInfo> executors = get_executors();

// ExecutorInfo impl
ExecutorInfo::ExecutorInfo(const char* exec, const char* fullname, const char* shortname, const char* lang, int version, const char* runtime, const char* const* args, bool stderr, get_executor_sig func) :
  exec(exec), fullname(fullname), shortname(shortname), language(lang), major_version(version), runtime(runtime), version_args(args), make_executor(func), stderr_version(stderr)
{}

std::string ExecutorInfo::get_info() {
  if(has_version)
    return info;

  has_version = true;

  int fd = RUNTIME_FUNC(open("/tmp", O_RDWR | O_TMPFILE, 0600));
  
  config conf;
  conf.memory = 128 * 1024 * 1024;
  conf.nproc = -1;
  conf.timelimit = 2;
  conf.fsize = EXECUTOR_INFO_LEN;
  conf.pstdin = null_read_fd;
  
  if(stderr_version) {
    conf.pstderr = fd;
  }
  else {
    conf.pstdout = fd;
  }
  
  SharedProcessResult shres;
  
  const char* null[] = {0};
  
  InsecureProcess proc(exec, version_args, null, conf, shres);
  
  proc.launch();
  
  // if they error, return that to user
  if((*shres).death_type != DEATH_NORMAL || (*shres).exit_info)
    info = (*shres).info;
  else
    info = read_from_file(fd);
    
  // success! 
  return info;
}