// File for various grading constants
// Primarily these are the file names and environment variables

#pragma once

// Environment variable names
#define CODE_FILE_ENV_NAME       "code_file"
#define IN_FILE_ENV_NAME         "input_file"
#define USER_OUT_FILE_ENV_NAME   "user_output_file"
#define JUDGE_OUT_FILE_ENV_NAME  "judge_output_file"
#define POINTS_ENV_NAME    "points"
#define SUITE_NUM_ENV_NAME "suite"
#define CASE_NUM_ENV_NAME  "case"


// File names
#define CODE_FILE_NAME      "main"
#define INPUT_FILE_NAME     "test.in"
#define USER_OUT_FILE_NAME  "user.out"
#define JUDGE_OUT_FILE_NAME "judge.out"


// Full Environment variables
#define IN_FILE_ENV         IN_FILE_ENV_NAME "=" INPUT_FILE_NAME
#define USER_OUT_FILE_ENV   USER_OUT_FILE_ENV_NAME "=" USER_OUT_FILE_NAME
#define JUDGE_OUT_FILE_ENV  JUDGE_OUT_FILE_ENV_NAME "=" JUDGE_OUT_FILE_NAME