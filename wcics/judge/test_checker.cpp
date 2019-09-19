#include <stdio.h>

#include "checkers/builtin_checkers.hpp"

int main() {
	const char* s1 = "a e c d b";
	const char* s2 = "c\t\n a \t \n b e d";
	
	printf("%d\n", builtin_check("unordered", s1, s2, "-10000"));
}
