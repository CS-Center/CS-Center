#include <string.h>
#include <vector>
#include <pybind11/pytypes.h>
#include <pybind11/stl.h>

namespace py = pybind11;

class CheckerInfo {
public:
  const char* const id;
  
  const char* const desc;
  
  const bool has_arg;
  
  CheckerInfo(const char* id, const char* desc, bool has_arg) : id(id), desc(desc), has_arg(has_arg) {}
};

const char* standard_desc = R"(This is the default checker. 

This checker has the same effect as tokenizing the input and ensuring that each token is identical.

The reason that it is not exactly that is for a performance boost. For details, consult the source code.

This checker should be sufficient for most problems.)";


const char* identical_desc = R"(As the name implies, this checker only accepts the output if the two outputs match exactly.

This checker is quite strict for most problems, and as such is not the default. However, it may sometimes be desired.)";


const char* standard_precision_desc = R"(This checker is most useful for questions with floating-point outputs. 

The input is first tokenized, and each token must either:

1. Match exactly or

2. Both be floats and be "close enough"

The definition of close enough depends on the floating-point checker. The standard precision checker compares relative and absolute error. If either are within the epsilon, the checker returns AC.

This checker (and the other floating point checkers) take exactly one argument, the precision. This is a floating-point integer to represent the epsilon. The actual epsilon is calculated as $ 10^{-arg} $. The reccomended value is 9.
)";


const char* relative_precision_desc = "This checker is the same as the standard precision checker, but it compares relative error only.";


const char* absolute_precision_desc = "This checker is the same as the relative precision checker, but uses absolute error.";


const char* unordered_desc = R"(This checker is helpful when the output can be in any order. This checker accepts the output if the tokenized output has the same content as the expected output. 

This means that the output tokens can be in any order, as long as they are the same tokens.)";


CheckerInfo standard_info("standard", standard_desc, 0);

CheckerInfo identical_info("identical", identical_desc, 0);

CheckerInfo standard_precision_info("standard_precision", standard_precision_desc, 1);

CheckerInfo relative_precision_info("relative_precision", relative_precision_desc, 1);

CheckerInfo absolute_precision_info("absolute_precision", absolute_precision_desc, 1);

CheckerInfo unordered_info("unordered", unordered_desc, 0);

std::vector<CheckerInfo> checkers = { 
  standard_info, 
  identical_info, 
  standard_precision_info, 
  relative_precision_info, 
  absolute_precision_info, 
  unordered_info 
};

bool has_checker(const char* id) {
  for(const CheckerInfo& c : checkers)
    if(strcmp(id, c.id) == 0)
      return 1;
      
  return 0;
}

PYBIND11_MODULE(py_checkers, m) {
  py::class_<CheckerInfo>(m, "CheckerInfo")
    .def_readonly("id", &CheckerInfo::id)
    .def_readonly("desc", &CheckerInfo::desc)
    .def_readonly("has_arg", &CheckerInfo::has_arg)
    .def("__repr__", [](const CheckerInfo& ci) {
      py::str r = "<CheckerInfo id=%s>";
      return r.attr("__mod__")(ci.id);
    });
    
  m.attr("checkers") = checkers;
  
  m.def("has_checker", &has_checker);
}