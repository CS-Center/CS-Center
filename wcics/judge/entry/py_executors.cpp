#include <string.h>
#include <pybind11/pytypes.h>
#include <pybind11/stl.h>

#include "executors/executor_info.hpp"

namespace py = pybind11;

bool has_executor(const char* shortname) {
  for(ExecutorInfo& obj : executors)
    if(strcmp(obj.shortname, shortname) == 0)
      return 1;

  return 0;
}

PYBIND11_MODULE(py_executors, m) {
  py::class_<ExecutorInfo>(m, "ExecutorInfo")
    .def_readonly("fullname", &ExecutorInfo::fullname)
    .def_readonly("shortname", &ExecutorInfo::shortname)
    .def_readonly("language", &ExecutorInfo::language)
    .def_readonly("runtime", &ExecutorInfo::runtime)
    .def_property_readonly("info", &ExecutorInfo::get_info)
    .def_property_readonly("args", [](const ExecutorInfo& ei) {
      py::list r;
      
      for(int i = 0; ei.version_args[i]; i++)
        r.attr("append")(ei.version_args[i]);
        
      return r;
    })
    .def("__repr__", [](const ExecutorInfo& ei) {
      py::str r = "<ExecutorInfo name=%s>";
      return r.attr("__mod__")(ei.shortname);
    })
    .def("__str__", [](const ExecutorInfo& ei) {
      py::str r = "<ExecutorInfo name=%s>";
      return r.attr("__mod__")(ei.shortname);
    });

  m.attr("executors") = executors;
  m.def("has_executor", &has_executor);
}