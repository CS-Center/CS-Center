#pragma once

#include <vector>
#include <string>

typedef long long ll;

// This file serves as a C++ interface to the database information

// cases
struct case_info {
  ll id;
  
  std::string input, output;
  
  case_info() = default;
  // construct based on primary keys
  case_info(ll id, int&);
};

struct suite_info {
  ll id;
  
  int points;
  std::vector<case_info> cases;
  
  suite_info() = default;
  suite_info(ll id, int&);
};

struct problem_info {
  int id;
  
  long double timelimit;
  long memory_limit;
  
  std::vector<suite_info> suites;
  
  std::string checker_id, checker_code;
  
  // this is the method of grading, either "partial", "full", or "sample_partial"
  std::string grading_id;
  
  problem_info() = default;
  problem_info(int id, int&);
};

struct submission_info {
  ll id;
  
  std::string code, lang;
  
  problem_info problem;
  
  submission_info() = default;
  submission_info(ll sid, int&);
};

struct sub_case_info {
  const char* verdict;
  const char* clipped_output;
  const char* info;
  int points;
  
  ll psid, sid;
  
  int commit();
};

// on normal submission execution, verdict should be empty
int complete_submission(ll sid, const char* verdict, const char* compilation_output = "");