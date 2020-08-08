#include <map>
#include <string>
#include <ctype.h>
#include <stack>
#include <cstdlib>
#include <cstdio>
#include <iostream>

#include "utils/debug.hpp"
#include "utils/files.hpp"
#include "executors/executor_factory.hpp"

#define CONF_FILE_DIR "executors/executor_ini"
#define CONF_FILE_NAME "executors.conf"

std::map<char, char> id_replace = {
	{ '+', 'p' },
};

std::string to_id(std::string str) {
	for(unsigned int i = 0; i < str.size(); i++) {
		char nc = tolower(str[i]);

		if(id_replace.count(nc))
			nc = id_replace[nc];

		str[i] = nc;
	}

	return str;
}

std::string lower(std::string str) {
	for(unsigned int i = 0; i < str.size(); i++) {
		char nc = tolower(str[i]);

		str[i] = nc;
	}

	return str;
}

std::string filepart(std::string val) {
	int ind = val.size();

	while(ind > 0 && val[ind - 1] != '/') ind--;

	return val.substr(ind);
}

typedef std::string (*conf_fn) (std::string);

std::map<std::string, conf_fn> ec_funcs = {
	{ "TO_ID", to_id },
	{ "LOWER", lower },
	{ "FILEPART", filepart }
};

std::map<std::string, int> ec_consts = {
	{ "COMPILED_FILENAME", econf::compiled_filename },
	{ "COMPILED_FILEPATH", econf::compiled_filepath },
	{ "SOURCE_FILENAME", econf::source_filename },
	{ "SOURCE_FILEPATH", econf::source_filepath },
	{ "BASE_FILE", econf::base_file },
	{ "NO_COMPILER", econf::no_compiler },
};

std::string f_content;

unsigned int f_index = 0;
int line = 1, col = 0;

bool is_id_char(char c) {
	char lc = tolower(c);

	return isalnum(lc) || (lc == '_');
}

void unget(const std::string& str = f_content, unsigned int& pos = f_index) {
	if(pos >= str.size()) {} // Legacy, should remote the str param

	pos--;
}

char gc(const std::string& str = f_content, unsigned int& pos = f_index) {
	if(pos >= str.size()) {
		pos++;
		return 0;
	};

	char c = str[pos++];

	if(&str == &f_content) {
		if(c == '\n') {
			line++;
			col = 0;
		}
		col++;
	}

	return c;
}

std::string read_id(const std::string& str = f_content, unsigned int& pos = f_index) {
	std::string ret;

	char c = gc(str, pos);
	while(is_id_char(c)) {
		ret += c;

		c = gc(str, pos);
	}

	unget(str, pos);

	return ret;
}

std::string rstrip(std::string str) {
	unsigned int ind = str.size();

	while(ind > 0 && isspace(str[ind - 1]))
		ind--;

	return str.substr(0, ind);
}

std::string lstrip(std::string str) {
	unsigned int ind = 0;
	while(ind < str.size() && isspace(str[ind]))
		ind++;

	return str.substr(ind);
}

std::string strip(std::string str) {
	return lstrip(rstrip(str));
}

std::vector<std::string> split(std::string str) {
	std::vector<std::string> ret;
	ret.push_back("");

	for(char c : str) {
		if(isspace(c)) {
			if(ret.back() != "") {
				ret.push_back("");
			}
		}
		else
			ret.back() += c;
	}

	return ret;
}

typedef std::map<std::string, std::string> var_context;

void trim(const std::string& str = f_content, unsigned int& pos = f_index) {
	char c;

	do {
		c = gc(str, pos);
	} while(c && isspace(c));

	unget(str, pos);
}

std::string outchar(char c) {
	if(isprint(c))
		return std::string("'") + c + "'";

	else
		return std::string("\\") + std::to_string(int(c));
}

void expect(char ec, const std::string& str = f_content, unsigned int& pos = f_index) {
	char sc = gc(str, pos);

	if(sc != ec) {
		fprintf(stderr, "Error when parsing config. Line %d column %d: Expected %s, got %s.", line, col, outchar(ec).c_str(), outchar(sc).c_str());

		exit(-1);
	}
}

std::string read_val() {
	std::string ret;

	char c = gc();
	while(c != '\n') {
		ret += c;

		c = gc();
	}

	return ret;
}

std::map<std::string, int> color;
std::map<std::string, econf_str> cache;

econf_str getval(var_context& vctx, const std::string& key);

econf_str evaluate(var_context& vctx, const std::string& val) {
	if(val[0] == '.') {
		std::string cname = val.substr(1);

		if(!ec_consts.count(cname)) {
			fprintf(stderr, "Error: Constant '%s' not found when evaluating '%s' on line %d.\n", cname.c_str(), val.c_str(), line);

			exit(-1);
		}

		return ec_consts[cname];
	}
	else {
		unsigned int index = 0;

		std::string ret;

		while(true) {
			char c = gc(val, index);

			std::string sub_id, f_name, param;

			switch(c) {
				case '\\':
					ret += gc(val, index);

					break;

				case '$':
					sub_id = read_id(val, index);

					if(color[sub_id] == 1) {
						fprintf(stderr, "Recursion error when trying to resolve '%s' on line %d.\n", val.c_str(), line);
					}

					ret += getval(vctx, sub_id).str;

					break;

				case '@':
					f_name = read_id(val, index);

					trim(val, index);
					expect('(', val, index);
					trim(val, index);

					param = read_id(val, index);

					trim(val, index);
					expect(')', val, index);

					if(color[param] == 1) {
						fprintf(stderr, "Recursion error when trying to resolve '%s' on line %d.\n", val.c_str(), line);
					}

					ret += ec_funcs[f_name](getval(vctx, param).str);

					break;

				case 0:
					return ret;

				default:
					ret += c;
			}
		}
	}
}

econf_str getval(var_context& vctx, const std::string& key) {
	if(cache.count(key) == 0) {
		color[key] = 1;

		std::string& val = vctx[key];

		cache[key] = evaluate(vctx, val);
	}
	color[key] = 2;

	return cache[key];
}

std::vector<econf_str> getvecval(var_context& vctx, const std::string& key) {
	std::string val = vctx[key];

	std::vector<std::string> tkns = split(val);

	std::vector<econf_str> ret;

	for(std::string& s : tkns) {
		ret.push_back(evaluate(vctx, s));
	}

	return ret;
}

std::vector<executor_config> do_export(var_context& vctx) {
	color = std::map<std::string, int>();
	cache = std::map<std::string, econf_str>();

	std::vector<executor_config> ret;

	if(vctx.count("VERSIONS") && !vctx.count("VERSION")) {
		var_context vcpy = vctx;

		vcpy.erase("VERSIONS");

		for(std::string v : split(vctx["VERSIONS"])) {
			vcpy["VERSION"] = v;

			for(executor_config ec : do_export(vcpy)) {
				ret.push_back(ec);
			}
		}
	}
	else {
		executor_config eret;

		eret.compiler_exec = getval(vctx, "COMPILER_EXEC");
		eret.compiler_args = getvecval(vctx, "COMPILER_ARGS");
		eret.source_ext = getval(vctx, "SOURCE_EXT").getstr();

		eret.interpreter_exec = getval(vctx, "INTERPRETER_EXEC");
		eret.interpreter_args = getvecval(vctx, "INTERPRETER_ARGS");
		eret.compiled_ext = getval(vctx, "COMPILED_EXT").getstr();

		eret.id = getval(vctx, "ID").getstr();
		eret.fullname = getval(vctx, "FULLNAME").getstr();

		eret.language_name = getval(vctx, "LANGUAGE_NAME").getstr();
		eret.language_id = getval(vctx, "LANGUAGE_ID").getstr();

		eret.runtime_name = getval(vctx, "RUNTIME_NAME").getstr();
		eret.runtime_id = getval(vctx, "RUNTIME_ID").getstr();

		eret.version = getval(vctx, "VERSION").getstr();

		eret.nproc = std::stoi(getval(vctx, "NPROC").getstr());

		ret.push_back(eret);
	}

	return ret;
}

std::vector<executor_config> parse_config(var_context vctx = var_context()) {
	char lastbrac = 0;

	std::vector<executor_config> ret;

	while(true) {
		trim();
		char c = gc();

		switch(c) {
			case '#':
				read_val();
				break;

			case '{':
				for(executor_config ec : parse_config(vctx)) {
					ret.push_back(ec);
				}

				lastbrac = '{';
				break;

			case '}':
				if(lastbrac == 0) {
					for(executor_config ec : do_export(vctx)) {
						ret.push_back(ec);
					}
				}

			case 0:
				return ret;

			default:
				unget();
				std::string varname = read_id();

				trim();
				expect('=');

				std::string val = read_val();

				vctx[varname] = strip(val);
		}
	}
}

std::vector<ExecutorFactory> get_factories() {
	f_content = read_from_file(CONF_FILE_DIR "/" CONF_FILE_NAME);

	std::vector<executor_config> ecs = parse_config();

	std::vector<ExecutorFactory> efs;

	for(executor_config& ec : ecs) {
		std::cout << ec.fullname << std::endl;

		efs.push_back(ExecutorFactory(ec));
	}

	return efs;
}

std::vector<ExecutorFactory> executor_factories = get_factories();
