#pragma once

// get length of null terminated char* array
int arrlen(const char* const*);

// copy the arguments from the first item into the second
// unti the first item contains NULL
// the second argument is incremented to the new free space in the buffer
void copy_args(const char* const*, const char** &);

// the first two arguments shall end with a NULL pointer
// if len1 is the length of the first argument, excluding the null pointer
// and len2 is the length of the first argument, excluding the null pointer
// then the last argument shall contain enough space for len1 + len2 + 1 pointers
// The filled in buffer will contain references to pointers in the first two arguments
void cat_args(const char* const*, const char* const*, const char**);

// similar to cat_args, but concatenates the executable with the arguments
void add_executable(const char*, const char* const*, const char**);

extern char** blank_args;