#include "executor_factory.hpp"
#include "utils/debug.hpp"

// ExecutorFactory impl
ExecutorFactory::ExecutorInfo(executor_config ec) : ec(ec) {}

econf_str::econf_str(int x) : val(x), str(0) {}
econf_str::econf_str(const char* s) : val(econf::null), str(s) {}

ExecutorFactory& get_factories(const char* id) {
	for(ExecutorFactory& ef : executor_factories) {
		if(id == ef.ec.id) return ef;
	}
	
	RUNTIME_FUNC(-1);
}