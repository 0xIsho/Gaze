function (append_common_compiler_options TARGET)

	# Compile options ---------------------------------------------------------

	list(APPEND COMMON_COMPILE_OPTIONS
		# Debug configuration
		$<$<CONFIG:Debug>:
			$<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:
				-Og
				-ftrapv
			>
		>
		# Release configuration
		$<$<CONFIG:Release>:
			$<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:
				-ftree-vectorize
				-fstack-protector-strong
				-fstack-clash-protection
				-Wdisabled-optimization
			>
			$<$<CXX_COMPILER_ID:MSVC>:
				/sdl # https://learn.microsoft.com/en-us/cpp/build/reference/sdl-enable-additional-security-checks
			>
		>
	)

	# Warnings ----------------------------------------------------------------

	list(APPEND COMMON_COMPILE_WARNINGS
		$<$<CXX_COMPILER_ID:MSVC>:
			/W4          # All reasonable warnings
			# /WX          # Treat warnings as errors
			/permissive- # Enforces standards conformance
			/w14242      # 'identfier': conversion from 'type1' to 'type2', possible loss of data
			/w14254      # 'operator': conversion from 'type1:field_bits' to 'type2:field_bits', possible loss of data
			/w14263      # 'function': member function does not override any base class virtual member function
			/w14265      # 'classname': class has virtual functions, but destructor is not virtual instances of this class may not be destructed correctly
			/w14287      # 'operator': unsigned/negative constant mismatch
			/we4289      # Nonstandard extension used: 'variable': loop control variable declared in the for-loop is used outside the for-loop scope
			/w14296      # 'operator': expression is always 'boolean_value'
			/w14311      # 'variable': pointer truncation from 'type1' to 'type2'
			/w14545      # Expression before comma evaluates to a function which is missing an argument list
			/w14546      # Function call before comma missing argument list
			/w14547      # 'operator': operator before comma has no effect; expected operator with side-effect
			/w14549      # 'operator': operator before comma has no effect; did you intend 'operator'?
			/w14555      # Expression has no effect; expected expression with side-effect
			/w14619      # Pragma warning: there is no warning number 'number'
			/w14640      # Enable warning on thread un-safe static member initialization
			/w14826      # Conversion from 'type1' to 'type_2' is sign-extended. This may cause unexpected runtime behavior.
			/w14905      # Wide string literal cast to 'LPSTR'
			/w14906      # String literal cast to 'LPWSTR'
			/w14928      # Illegal copy-initialization; more than one user-defined conversion has been implicitly applied
		>
		$<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:
			# -Werror                   # Treat warnings as errors
			-Wall -Wextra             # Reasonable and standard
			-pedantic                 # Warn on language extensions
			-Wshadow                  # Warn the user if a variable declaration shadows one from a parent context
			-Wnon-virtual-dtor        # Warn the user if a class with virtual functions has a non-virtual destructor
			-Wold-style-cast          # Warn for c-style casts
			-Wcast-align              # Warn for potential performance problem casts
			-Wunused                  # Warn on anything being unused
			-Woverloaded-virtual      # Warn if you overload (not override) a virtual function
			-Wconversion              # (all versions of GCC, Clang >= 3.2) warn if non-standard C++ is used
			-Wsign-conversion         # Warn on type conversions that may lose data
			-Wstrict-overflow=2       # Warn about cases where the compiler optimizes based on the assumption that signed overflow does not occur
			-Wdouble-promotion        # (Clang all versions, GCC >= 4.3) warn on sign conversions
			-Wformat=2                # Warn on security issues around functions that format output (ie printf)
			-Wfloat-equal             # Warning: comparing floating point with == or != is unsafe
			-Wlogical-not-parentheses # Warning: logical not is only applied to the left hand side of comparison
			-Wno-unknown-pragmas      # Warning: unknown pragma ignored (We are using some "unknown" pragmas, so ignore this one)
			$<$<CXX_COMPILER_ID:GNU>:
				-Wmisleading-indentation # (only in GCC >= 6.0) warn if indentation implies blocks where blocks do not exist
				-Wduplicated-cond        # (only in GCC >= 6.0) warn if if / else chain has duplicated conditions
				-Wduplicated-branches    # (only in GCC >= 7.0) warn if if / else branches have duplicated code
				-Wlogical-op             # (only in GCC) warn about logical operations being used where bitwise were probably wanted
				-Wnull-dereference       # (only in GCC >= 6.0) warn if a null dereference is detected
				-Wuseless-cast           # (only in GCC >= 4.8) warn if you perform a cast to the same type
			>
		>
	)

	# Compile definitions -----------------------------------------------------

	list(APPEND COMMON_COMPILE_DEFINITIONS
		$<$<CXX_COMPILER_ID:MSVC>:
			NOMINMAX            # Do not define the `min` and `max` macros
			# If the following definitions happen to break the build due to a missing
			# header, that header may be included separately.
			WIN32_LEAN_AND_MEAN # Disable some of the less used headers. Improves build times
			VC_EXTRALEAN        # Disable some more headers. Improves build times
		>
	)

	# Link options ------------------------------------------------------------

	list(APPEND COMMON_LINK_OPTIONS
		$<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:
			"LINKER:-z,now"
			"LINKER:-z,relro"
			"LINKER:-z,noexecstack"
		>
	)

	# -------------------------------------------------------------------------

	target_compile_options(${TARGET}
		PRIVATE
			${COMMON_COMPILE_OPTIONS}
			${COMMON_COMPILE_WARNINGS}
	)
	target_compile_definitions(${TARGET}
		PRIVATE
			${COMMON_COMPILE_DEFINITIONS}
	)
	target_link_options(${TARGET}
		PRIVATE
			${COMMON_LINK_OPTIONS}
	)
endfunction()
