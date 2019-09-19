#include <string.h>

#include "executors/clang/clang_executor.hpp"
#include "executors/clang++/clangpp_executor.hpp"
#include "executors/cpython/cpython_executor.hpp"
#include "executors/g++/gpp_executor.hpp"
#include "executors/gcc/gcc_executor.hpp"
#include "executors/java/java_executor.hpp"
#include "executors/pypy/pypy_executor.hpp"

#include "sandbox/process/insecure_process.hpp"

#include "utils/pipes.hpp"

#include "executor_info.hpp"

#define DEF_INFO_FUNC(name) Executor* New##name##Executor(const char* code, const char* file, const char* const* extra_args, const char* const* env, config& conf, AsyncCommunicator& acomm, FileAccessChecker& fac, SharedProcessResult& res) {\
  return new name##Executor(code, file, extra_args, env, conf, acomm, fac, res); \
}

#ifdef C_ENABLED
  
  #ifdef CLANG_ENABLED
  
    #ifdef CLANG_90_ENABLED
      DEF_INFO_FUNC(Clang90);
    #endif
    
    #ifdef CLANG_99_ENABLED
      DEF_INFO_FUNC(Clang99);
    #endif
    
    #ifdef CLANG_11_ENABLED
      DEF_INFO_FUNC(Clang11);
    #endif
    
  #endif
  
  #ifdef GCC_ENABLED
  
    #ifdef GCC_90_ENABLED
      DEF_INFO_FUNC(GCC90);
    #endif
    
    #ifdef GCC_99_ENABLED
      DEF_INFO_FUNC(GCC99);
    #endif
    
    #ifdef GCC_11_ENABLED
      DEF_INFO_FUNC(GCC11);
    #endif
    
  #endif
  
#endif

#ifdef CPP_ENABLED

  #ifdef CLANGPP_ENABLED
  
    #ifdef CLANGPP_98_ENABLED
      DEF_INFO_FUNC(Clangpp98);
    #endif
    
    #ifdef CLANGPP_03_ENABLED
      DEF_INFO_FUNC(Clangpp03);
    #endif
    
    #ifdef CLANGPP_11_ENABLED
      DEF_INFO_FUNC(Clangpp11);
    #endif
    
    #ifdef CLANGPP_14_ENABLED
      DEF_INFO_FUNC(Clangpp14);
    #endif
    
    #ifdef CLANGPP_17_ENABLED
      DEF_INFO_FUNC(Clangpp17);
    #endif
    
  #endif
  
  #ifdef GPP_ENABLED
  
    #ifdef GPP_98_ENABLED
      DEF_INFO_FUNC(Gpp98);
    #endif
    
    #ifdef GPP_03_ENABLED
      DEF_INFO_FUNC(Gpp03);
    #endif
    
    #ifdef GPP_11_ENABLED
      DEF_INFO_FUNC(Gpp11);
    #endif
    
    #ifdef GPP_14_ENABLED
      DEF_INFO_FUNC(Gpp14);
    #endif
    
    #ifdef GPP_17_ENABLED
      DEF_INFO_FUNC(Gpp17);
    #endif
    
  #endif
  
#endif

#ifdef JAVA_ENABLED

  #ifdef JAVA_8_ENABLED
    DEF_INFO_FUNC(Java8);
  #endif
  
  #ifdef JAVA_11_ENABLED
    DEF_INFO_FUNC(Java11);
  #endif
  
#endif

#ifdef PYTHON_ENABLED

  #ifdef CPYTHON_ENABLED
  
    #ifdef CPYTHON2_ENABLED
      DEF_INFO_FUNC(CPython2);
    #endif
    
    #ifdef CPYTHON3_ENABLED
      DEF_INFO_FUNC(CPython3);
    #endif
    
  #endif
  
  #ifdef PYPY_ENABLED
  
    #ifdef PYPY2_ENABLED
      DEF_INFO_FUNC(PyPy2);
    #endif
    
    #ifdef PYPY3_ENABLED
      DEF_INFO_FUNC(PyPy3);
    #endif
  
  #endif
  
#endif

#define ADD_INFO(name, args...) res.push_back(ExecutorInfo(args, New##name##Executor));

std::vector<ExecutorInfo> get_executors() {
  std::vector<ExecutorInfo> res;
  
  #ifdef C_ENABLED
  
    #ifdef CLANG_ENABLED
      const char* clang_args[] = {"clang", "-dumpversion", 0};

      #ifdef CLANG_90_ENABLED
        ADD_INFO(Clang90, CLANG_PATH, "Clang90", "clang90", "C", 90, "Clang", clang_args, 0);
      #endif

      #ifdef CLANG_99_ENABLED
        ADD_INFO(Clang99, CLANG_PATH, "Clang99", "clang99", "C", 99, "Clang", clang_args, 0);
      #endif

      #ifdef CLANG_11_ENABLED
        ADD_INFO(Clang11, CLANG_PATH, "Clang11", "clang11", "C", 11, "Clang", clang_args, 0);
      #endif

    #endif

    #ifdef GCC_ENABLED

      const char* gcc_args[] = {"gcc", "-dumpversion", 0};

      #ifdef GCC_90_ENABLED
        ADD_INFO(GCC90, GCC_PATH, "GCC90", "gcc90", "C", 90, "GCC", gcc_args, 0);
      #endif

      #ifdef GCC_99_ENABLED
        ADD_INFO(GCC99, GCC_PATH, "GCC99", "gcc99", "C", 99, "GCC", gcc_args, 0);
      #endif

      #ifdef GCC_11_ENABLED
        ADD_INFO(GCC11, GCC_PATH, "GCC11", "gcc11", "C", 11, "GCC", gcc_args, 0);
      #endif

    #endif

  #endif

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

const char* ExecutorInfo::get_info() {
  if(has_version)
    return info;

  has_version = true;

  communicate_fd in, out, err;
  
  int pipe_stdout = PIPE_NULL, pipe_stderr = PIPE_NULL;
  if(stderr_version) {
    err.buffer = info;
    err.length = EXECUTOR_INFO_LEN;
    pipe_stderr = PIPE_NORMAL;
  }
  else {
    out.buffer = info;
    out.length = EXECUTOR_INFO_LEN;
    pipe_stdout = PIPE_NORMAL;
  }
  
  config conf;
  conf.memory = 128 * 1024 * 1024;
  conf.nproc = -1;
  conf.timelimit = 2;
  
  int status = 0;
  Communicator comm(in, out, err, conf, PIPE_NULL, pipe_stdout, pipe_stderr, status);
  
  if(status) {
    fprintf(stderr, "Failed to get executor info for %s", shortname);
    perror("Communicator");
    
    return "Error getting info: Communicator couldn't be constructed!";
  }
  
  AsyncCommunicator acomm(comm);
  
  SharedProcessResult shres(status);
  
  if(status) {
    fprintf(stderr, "Failed to get executor info for %s", shortname);
    perror("SharedProcessResult");
    
    return "Error getting info: SharedProcessResult couldn't be constructed!";
  }
  
  const char* null[] = {0};
  
  InsecureProcess proc(acomm, exec, version_args, null, conf, shres);
  
  proc.launch();
  
  // if they error, return that to user
  if((*shres).death_type != DEATH_NORMAL || (*shres).exit_info)
    strncpy(info, (*shres).info, EXECUTOR_INFO_LEN);
  
  // success!
  return info;
}