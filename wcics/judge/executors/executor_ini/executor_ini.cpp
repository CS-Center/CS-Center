#include <map>
#include <string>
#include <ctype.h>
#include <stack>

#include "utils/debug.hpp"
#include "utils/files.hpp"
#include "executors/executor_factory.hpp"

#define CONF_FILE_DIR "executors/executor_ini"
#define CONF_FILE_NAME "executors.conf"

std::string trim(std::string str) {	
	int start = 0;
	
	while(start < str.size() && isspace(str[start]))
		start++;
		
	if(start == str.size()) return "";
		
	int end = str.size() - 1;
	
	while(end >= start && isspace(str[end]))
		end--;
		
	return str.substr(start, end - start + 1);
}

std::map<char, char> replace = {
	{ '+', 'p' },
};

std::string to_id(std::string str) {
	std::string ret;
	
	for(char c : str) {
		char nc = tolower(c);
		
		if(replace.count(nc))
			nc = replace[nc];
	
		ret += nc;
	}
	
	return ret;
}

std::vector<ExecutorFactory> parse_config(std::vector<std::string>& lines);

std::vector<ExecutorFactory> get_factories() {
	std::string content = read_from_file(CONF_FILE_DIR "/" CONF_FILE_NAME);
	
	std::vector<std::string> lines = { "" };
	
	for(char c : content) {
		if(c == '\n') {
			lines.back() = trim(lines.back());
			
			if(lines.back().size() == 0 || lines.back()[0] == '#')
				lines.pop_back();
		
			lines.push_back("");
		}
	
		else
			lines.back() += c;
	}
	
	return parse_config(lines);
}

std::vector<ExecutorFactory> executor_factories = get_factories();

typedef std::map<std::string, std::string> var_context;

typedef econf_str (*vctx_func) (var_context);

std::map<std::string, int> int_consts = {
	{ "SOURCE_FILENAME", econf::source_filename },
	{ "SOURCE_FILEPATH", econf::srouce_filepath },
	
	{ "COMPILED_FILENAME", econf::compiled_filename },
	{ "COMPILED_FILEPATH", econf::compiled_filepath },
	
	{ "BASE_FILE", econf::base_file },
};

std::map<std::string, vctx_func> default_funcs = {
	{ "LANGUAGE_ID", [](var_context vctx) { 
		
	}},
};

std::vector<ExecutorFactory> parse_config(std::vector<std::string>& lines) {

}


