// This file is slightly larger than is normal, the reason is for more efficient compilation, as pybind11 takes a _long time_ to compile

#include <stdlib.h>
#include <stdio.h>
#include <utility>

#include <pybind11/pybind11.h>

#include "db_info.hpp"

namespace py = pybind11;
using namespace py::literals;

#define MAIN_MODULE "wcics"

py::module db_models, db_obj;
bool _models_initialized = false, _db_initialized = false;

// helper to (possibly) import the module and return the specified model
int init_models() {
  // if this throws, its fatal, so we dont really mind
  if(!_models_initialized) {
    _models_initialized = true;
    try {
      db_models = py::module::import(MAIN_MODULE ".database.models");
    } catch(py::error_already_set err) {
      // fatal error
      fputs("Fatal error: could not import main module's database models", stderr);
      
      return -1;
    }
  }
  
  return 0;
}

int init_db() {
  if(!_db_initialized) {
    _db_initialized = true;
    try {
      db_obj = py::module::import(MAIN_MODULE);
    } catch(py::error_already_set err) {
      fputs("Fatal error: could not import the main module's database object", stderr);
      
      fprintf(stderr, "what(): '%s'", err.what());
      
      return -1;
    }
  }
  
  return 0;
}

int init_modules() {
  return init_db() == -1 ? -1 : init_models();
}

// utility to get query directly from model name
// if you didnt init the modules well screw you
py::object get_query(const char* name) { return db_models.attr("query"); }

void case_from_pyobj(case_info* c, const py::handle& obj) {
  c->id = obj.attr("id").cast<ll>();
  c->input = obj.attr("input").cast<std::string>();
  c->output = obj.attr("output").cast<std::string>();
}

case_info::case_info(ll id, int& status) : id(id) {
  if(init_modules()) {
    status = -1;
    return;
  }

  py::object result = get_query("ProblemCases").attr("filter_by")("id"_a = id).attr("first")();
  
  case_from_pyobj(this, result);
}

suite_info::suite_info(ll id, int& status) : id(id) {
  if(init_modules()) {
    status = -1;
    return;
  }

  py::object result = get_query("ProblemSuites").attr("filter_by")("id"_a = id).attr("first")();
  
  points = result.cast<int>();
  
  // get problem cases ordered by suite
  result = get_query("ProblemCases").attr("filter_by")("psid"_a = id).attr("order_by")("case").attr("all")();
  
  for(const py::handle o : result) {
    case_info c;
    case_from_pyobj(&c, o);
    
    cases.push_back(std::move(c));
  }
  
  status = 0;
}

problem_info::problem_info(int id, int& status) : id(id) {
  // here we get the problem, suites and cases for a total of 3 queries :D
  
  if(init_modules()) {
    status = -1;
    return;
  }
  
  py::object problem = get_query("Problems").attr("filter_by")("id"_a = id).attr("first")();
  py::object db_suites = get_query("ProblemSuites").attr("filter_by")("pid"_a = id).attr("order_by")("suite").attr("all")();
  py::object db_cases = get_query("ProblemCases").attr("filter_by")("pid"_a = id).attr("order_by")("suite", "case").attr("all")();
  
  if(problem.is_none()) {  
    fprintf(stderr, "Fatal error: problem with id %d does not exist!", id);
    
    status = -1;
    return;
  }
  
  checker_id = problem.attr("checker_id").cast<std::string>();
  checker_code = problem.attr("checker_code").cast<std::string>();
  
  grading_id = problem.attr("grading_id").cast<std::string>();
  
  timelimit = problem.attr("timelimit").cast<long double>();
  
  memory_limit = problem.attr("memory_limit").cast<long>();
      
  for(const py::handle& h : db_suites) {
    suites.emplace_back();
    suites.back().id = h.attr("id").cast<ll>();
    suites.back().points = h.attr("points").cast<int>();
  }
  
  int si = 0;
  for(const py::handle& h : db_cases) {
    ll psid = h.attr("psid").cast<ll>();
    
    if(psid != suites[si].id) {
      si++;
      
      if(suites[si].id != psid) {
        std::string pid = problem.attr("pid").cast<std::string>();
        fprintf(stderr, "Fatal Error: suites and cases of problem %s are configured incorrectly! Refusing to grade.", pid.c_str());
        
        status = -1;
        return;
      }
    }
    
    suites[si].cases.emplace_back();
    suites[si].cases.back().id = h.attr("id").cast<ll>();
    
    suites[si].cases.back().input = h.attr("input").cast<std::string>();
    suites[si].cases.back().output = h.attr("output").cast<std::string>();
  }
  
  status = 0;
}

submission_info::submission_info(ll id, int& status) : id(id) {
  if(init_modules()) {
    status = -1;
    return;
  }

  py::object sub = get_query("Submissions").attr("filter_by")("id"_a = id).attr("first")();
  
  code = sub.attr("code").cast<std::string>();
  lang = sub.attr("lang").cast<std::string>();
  
  problem = problem_info(sub.attr("pid").cast<int>(), status);
  
  if(status)
    return;
  
  status = 0;
}

int sub_case_info::commit() {
  if(init_modules())
    return -1;

  py::object obj = db_obj.attr("SubmissionCases")("sid"_a = sid, "psid"_a = psid, "verdict"_a = verdict, "info"_a = info, "points"_a = points, "clipped_output"_a = clipped_output);
  
  db_obj.attr("session").attr("add")(obj);
  
  db_obj.attr("session").attr("commit")();
  
  return 0;
}

int complete_submission(ll sid, const char* verdict, const char* compilation_output) {
  if(init_modules())
    return -1;

  py::object obj = get_query("Submissions")("id"_a = sid);
  
  obj.attr("verdict") = verdict;
  obj.attr("compilation_output") = compilation_output;

  db_obj.attr("session").attr("commit")();
  
  return 0;
}