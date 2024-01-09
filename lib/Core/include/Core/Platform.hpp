#ifndef GAZE_CORE_PLATFORM_HPP
#define GAZE_CORE_PLATFORM_HPP

#if defined(_WIN32)
	#define GAZE_PLATFORM_WINDOWS
	#define WIN32_LEAN_AND_MEAN
	#define _AMD64_
	#if defined(_WIN64)
		#define GAZE_PLATFORM_WIN64
	#else
		#error "x86 build are not supported!"
	#endif
#elif defined(__APPLE__) || defined(__MACH__)
	#define GAZE_PLATFORM_APPLE
	#include <TargetConditionals.h>
	#if TARGET_IPHONE_SIMULATOR == 1
		#define GAZE_PLATFORM_IOS_SIMULATOR
		#error "IPhone Simulator is not supported!"
	#elif TARGET_OS_IPHONE == 1
		#define GAZE_PLATFORM_IOS
		#error "IOS is not supported!"
	#elif TARGET_OS_MAC == 1
		#define GAZE_PLATFORM_MACOS
		#error "macOS is not supported!"
	#else
		#error "Unknown Apple platform"
	#endif
#elif defined(__ANDROID__)
	#define GAZE_PLATFORM_ANDROID
	#error "Android is not supported!"
#elif defined(__linux__)
	#define GAZE_PLATFORM_LINUX
#else
	#error "Unknown or unsupported platform!"
#endif

// Compiler/Toolchain
#if defined(__clang__)
#	define GAZE_COMPILER_CLANG 1
#	define GAZE_COMPILER_MAJOR __clang_major__
#	define GAZE_COMPILER_MINOR __clang_minor__
#	define GAZE_COMPILER_PATCH __clang_patchlevel__
#elif defined(__INTEL_COMPILER)
#	define GAZE_COMPILER_ICC 1
	// TODO: Version numbers
#elif defined(__GNUC__)
#	define GAZE_COMPILER_GNU 1
#	define GAZE_COMPILER_MAJOR __GNUC__
#	define GAZE_COMPILER_MINOR __GNUC_MINOR__
#	define GAZE_COMPILER_PATCH __GNUC_PATCHLEVEL__
#elif defined(_MSC_VER)
#	define GAZE_COMPILER_MSVC 1
#else
#	error "Unknown or unsupported compiler"
#endif

#if !defined(GAZE_COMPILER_MAJOR)
#	define GAZE_COMPILER_MAJOR 0
#endif
#if !defined(GAZE_COMPILER_MINOR)
#	define GAZE_COMPILER_MINOR 0
#endif
#if !defined(GAZE_COMPILER_PATCH)
#	define GAZE_COMPILER_PATCH 0
#endif

#endif // GAZE_CORE_PLATFORM_HPP
