#pragma once

#include "runtimes/cpp.hpp"

#if CPP_ENABLED
	#if GPP_ENABLED
		#define HAS_GPP 1

		#if GPP_98_ENABLED
			#define HAS_GPP_98 1
		#endif // GPP_98_ENABLED

		#if GPP_03_ENABLED
			#define HAS_GPP_03 1
		#endif // GPP_03_ENABLED

		#if GPP_11_ENABLED
			#define HAS_GPP_11 1
		#endif // GPP_11_ENABLED

		#if GPP_14_ENABLED
			#define HAS_GPP_14 1
		#endif // GPP_14_ENABLED

		#if GPP_17_ENABLED
			#define HAS_GPP_17 1
		#endif // GPP_17_ENABLED

	#endif // GPP_ENABLED

	#if CLANGPP_ENABLED
		#define HAS_CLANGPP 1

		#if CLANGPP_98_ENABLED
			#define HAS_CLANGPP_98 1
		#endif // CLANGPP_98_ENABLED

		#if CLANGPP_03_ENABLED
			#define HAS_CLANGPP_03 1
		#endif // CLANGPP_03_ENABLED

		#if CLANGPP_11_ENABLED
			#define HAS_CLANGPP_11 1
		#endif // CLANGPP_11_ENABLED

		#if CLANGPP_14_ENABLED
			#define HAS_CLANGPP_14 1
		#endif // CLANGPP_14_ENABLED

		#if CLANGPP_17_ENABLED
			#define HAS_CLANGPP_17 1
		#endif // CLANGPP_17_ENABLED

	#endif // CLANGPP_ENABLED

#endif // CPP_ENABLED
