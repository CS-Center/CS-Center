#include "c_cpp.hpp"
#include "executor_info.hpp"

c_cpp_config::update(executor_config& ec) {
	ec.id = id;
	ec.fullname = fullname;
	ec.compiler_args.back() = stdflag;
	ec.major_version = version;
}
	
std::vector<c_cpp_config> c_confs = {
#if C_ENABLED
	#if GCC_ENABLED
		{
			"gcc90",
			"C90 - GCC",
			"-std=c90",
			"90",
			GCC_90_ENABLED
		},
		{
			"gcc99",
			"C99 - GCC",
			"
		
	
#endif // C_ENABLED
};