#pragma once

#include "Platform.hpp"

#if defined(GAZE_COMPILER_CLANG) || defined(GAZE_COMPILER_GNU)
#	if defined(__has_builtin)
#		if __has_builtin(__builtin_expect)
#			define GAZE_EXPECT(expr, val) __builtin_expect(expr, val)
#		endif
#	endif
#	define GAZE_UNREACHABLE() __builtin_unreachable()
#elif defined(GAZE_COMPILER_MSVC)
#	define GAZE_UNREACHABLE() __assume(0)
#endif

#if !defined(GAZE_EXPECT)
#	define GAZE_EXPECT(expr, val) (expr)
#endif
#if !defined(GAZE_UNREACHABLE)
#	error "__builtin_unreachable() equivalent missing"
#endif
