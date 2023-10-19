#ifndef GAZE_DEBUG_ASSERT_HPP
#define GAZE_DEBUG_ASSERT_HPP

#include <cassert>
#include <string_view>

#define GAZE_ASSERT(cond, msg) if ((cond)) { } else ::Gaze::Debug::AssertFail(#cond, msg, __FILE__, __LINE__)

namespace Gaze::Debug {
	auto AssertFail(std::string_view expr, std::string_view msg, std::string_view file, int line) -> void;
}

#endif // GAZE_DEBUG_ASSERT_HPP
