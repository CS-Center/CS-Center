#pragma once

#include "runtimes/c.hpp"

#if C_ENABLED
	#if GCC_ENABLED
		#define HAS_GCC 1

		#if GCC_90_ENABLED
			#define HAS_GCC_90 1
		#endif // GCC_90_ENABLED

		#if GCC_99_ENABLED
			#define HAS_GCC_99 1
		#endif // GCC_99_ENABLED

		#if GCC_11_ENABLED
			#define HAS_GCC_11 1
		#endif // GCC_11_ENABLED

	#endif // GCC_ENABLED

	#if CLANG_ENABLED
		#define HAS_CLANG 1

		#if CLANG_90_ENABLED
			#define HAS_CLANG_90 1
		#endif // CLANG_90_ENABLED

		#if CLANG_99_ENABLED
			#define HAS_CLANG_99 1
		#endif // CLANG_99_ENABLED

		#if CLANG_11_ENABLED
			#define HAS_CLANG_11 1
		#endif // CLANG_11_ENABLED

	#endif // CLANG_ENABLED

#endif // C_ENABLED
