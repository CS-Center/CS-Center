#pragma once

#include "runtimes/python.hpp"

#if PYTHON_ENABLED
	#if CPYTHON_ENABLED
		#define HAS_CPYTHON 1

		#if CPYTHON2_ENABLED
			#define HAS_CPYTHON_2 1
		#endif // CPYTHON2_ENABLED

		#if CPYTHON3_ENABLED
			#define HAS_CPYTHON_3 1
		#endif // CPYTHON3_ENABLED

	#endif // CPYTHON_ENABLED

	#if PYPY_ENABLED
		#define HAS_PYPY 1

		#if PYPY2_ENABLED
			#define HAS_PYPY_2 1
		#endif // PYPY2_ENABLED

		#if PYPY3_ENABLED
			#define HAS_PYPY_3 1
		#endif // PYPY3_ENABLED

	#endif // PYPY_ENABLED

#endif // PYTHON_ENABLED
