#include "Debug/Assert.hpp"

#include <iostream>

namespace Gaze::Debug {
	auto AssertFail(std::string_view expr, std::string_view msg, std::string_view file, int line) -> void
	{
		std::cerr
			<< "*** Assertion Failure ***\n"
			<< "Expression: " << expr << '\n'
			<< "At:         " << file << ':' << line << '\n'
			<< "Message:    " << msg << '\n';

		std::abort();
	}
}
