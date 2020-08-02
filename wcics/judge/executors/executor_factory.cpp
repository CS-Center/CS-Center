#include "executor_factory.hpp"
#include "utils/debug.hpp"

// ExecutorFactory impl
ExecutorFactory::ExecutorFactory(executor_config ec) : ec(ec) {}

econf_str::econf_str() : val(econf::null), str("") {}
econf_str::econf_str(int x) : val(x), str("") {}
econf_str::econf_str(std::string s) : val(econf::null), str(s) {}

bool econf_str::operator==(int v) { return val == v; }

std::string econf_str::getstr() {
	if(val == econf::null)
		return str;

	RUNTIME_FUNC(-1);
}

ExecutorFactory& get_factory(const char* id) {
	for(ExecutorFactory& ef : executor_factories) {
		if(id == ef.ec.id) return ef;
	}

	RUNTIME_FUNC(-1);
}
