#pragma once

#include "runtimes/java.hpp"

#if JAVA_ENABLED
	#define HAS_JAVA 1
	
	#if JAVA_8_ENABLED
		#define HAS_JAVA_8 1
	#endif // JAVA_8_ENABLED
	
	#if JAVA_11_ENABLED
		#define HAS_JAVA_11 1
	#endif // JAVA_11_ENABLED
	
#endif // JAVA_ENABLED