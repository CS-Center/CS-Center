#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <string>
#include <vector>

#include "paths.hpp"
#include "utils/debug.hpp"

using namespace std;

string normalize_abs(string path) {  
	if(path[0] != '/') {
    errno = EINVAL;
		RUNTIME_FUNC(-1);
	}
  
  vector<string> items;
  
	for(char c : path) {
    if(c == '/') {    
      if(items.back() == ".") items.pop_back(); // useless item
      if(items.back() == "..") { // remove this item and the previous
        items.pop_back();
        
        if(!items.empty()) items.pop_back();
      }
    
      if(items.back() != "") items.push_back(""); // two slashes in a row
    }
    
    else
      items.back() += c;
  }
  
  if(items.empty()) return "/";
  
  string r;
  
  for(const string& s : items) {
    r += "/";
    r += s;
  }
  
  return r;
}

string fulljoin(string dir, string file) {
	if(file[0] == '/')
		return normalize_abs(file);
  
  return normalize_abs(dir + file);
}