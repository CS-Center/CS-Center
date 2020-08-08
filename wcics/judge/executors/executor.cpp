#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "sandbox/process/insecure_process.hpp"
#include "sandbox/process/secure_process/secure_process.hpp"
#include "executor.hpp"
#include "executor_factory.hpp"
#include "utils/files.hpp"
#include "utils/debug.hpp"
#include "utils/pipes.hpp"

bool Executor::is_compiled() {
	if(ec.compiler_exec == econf::no_compiler)
		return false;

	if(ec.compiler_exec == econf::null)
		return true;

	RUNTIME_ERROR();
}

const char* Executor::get_str(econf_str& ecf) {
	switch(ecf.val) {
		case econf::null:
			return ecf.str.c_str();

		case econf::base_file:
			return base_filename.c_str();

		case econf::source_filename:
			return source_filename.c_str();

		case econf::source_filepath:
			return source_filepath.c_str();

		case econf::compiled_filename:
			return compiled_filename.c_str();

		case econf::compiled_filepath:
			return compiled_filepath.c_str();

		default:
			RUNTIME_FUNC(-1);
	}
}

config Executor::get_compiler_config(int temp_fd) {
	config cconf;

	cconf.memory = ec.compiler_memory;
	cconf.timelimit = ec.compiler_timelimit;
	cconf.fsize = ec.compiler_max_file_size;
	cconf.nproc = -1;
	cconf.dir = conf.dir;
	cconf.pstdin = null_read_fd;
	cconf.pstdout = null_write_fd;
	cconf.pstderr = temp_fd;

	return cconf;
}

void Executor::compile() {
	if(!is_compiled()) return;

	vector<const char*> cargs;

	for(econf_str& ecstr : ec.compiler_args)
		cargs.push_back(get_str(ecstr));

	cargs.push_back(0);


	int temp_fd = get_temp_fd();

	config cconf = get_compiler_config(temp_fd);

	const char* null_env[] = {0};

	InsecureProcess proc(get_str(ec.compiler_exec), cargs.data(), null_env, cconf, compiler_res);

	proc.launch();

	compiler_output = read_from_file(temp_fd);

	RUNTIME_FUNC(close(temp_fd));
}

Executor::Executor(executor_config& ec, std::string code, const char* file, std::vector<const char*> extra_args, const char* const* env, config& conf, FileAccessChecker& fac, SharedProcessResult& res) :
	code(code),
	env(env),
	conf(conf),
	fac(fac),
	res(res),
	ec(ec),
  base_filename(file),
	source_filename(base_filename + ec.source_ext),
	source_filepath(conf.dir + ("/" + source_filename)),
	compiled_filename(base_filename + ec.compiled_ext),
	compiled_filepath(conf.dir + ("/" + compiled_filename))
{
	write_to_file(source_filepath.c_str(), code);

	for(econf_str& ecstr : ec.interpreter_args)
		args.push_back(get_str(ecstr));

	for(const char* str : extra_args)
		args.push_back(str);

	args.push_back(0);

	compile();
}

void Executor::cleanup() {
  if(has_cleanup)
    return;

  has_cleanup = true;

  RUNTIME_FUNC(unlink(source_filepath.c_str()));

	if(is_compiled())
		RUNTIME_FUNC(unlink(compiled_filepath.c_str()));
}

void Executor::launch() {
  SecureProcess proc(get_str(ec.interpreter_exec), args.data(), env, conf, res, fac);

  proc.launch();
}

process_result& Executor::get_result() {
	return *res;
}

Executor::~Executor() {
	cleanup();
}
