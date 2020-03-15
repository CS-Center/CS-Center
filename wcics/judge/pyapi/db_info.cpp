// This file includes some cpp files to split up the files nicely, however they share the loaded pybind11 header that takes upwards of 3 seconds to compie

#include <stdlib.h>
#include <stdio.h>
#include <utility>

#include <pybind11/pybind11.h>

#include "db_info.hpp"

namespace py = pybind11;
using namespace py::literals;

#define MAIN_MODULE "wcics"

#include "db_info/problem_info.cpp"
#include "db_info/submission_info.cpp"