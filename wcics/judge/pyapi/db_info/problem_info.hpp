#pragma once

#include <vector>
#include <string>

typedef long long ll;

// This file serves as a C++ interface to the problem database

struct ProblemCases {
  ll pid, sid, id;
  
  int casenum;
  
  string input, output;
  
  bool operator<(const ProblemCases& other);
  
  ProblemCases(ll id);
};

struct ProblemSuites {
  vector<ProblemCases> cases;
  
  ll id, pid;
  
  int suite, points;
  
  bool operator<(const ProblemSuites& other);
  
  ProblemSuites(ll id);
};

struct ProblemJudging {
  ll pid;
  
  string checker_id, checker_code, checker_lang, checker_arg;
  
  bool custom_interactor;
  
  string interactor_code, interactor_lang;
  
  vector<ProblemSuites> suites;
  
  ProblemJudging(ll pid);
};