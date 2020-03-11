#include <stdio.h>

#include "checkers/builtin_checkers.hpp"

int main() {
	const char* s1 = "-10     10";
	const char* s2 = "-10.000000005        12";
	
	printf("%d\n", builtin_check("relative_precision", s1, s2, "1"));
}
