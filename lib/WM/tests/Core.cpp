#include <catch2/catch.hpp>

#include "WM/Core.hpp"

TEST_CASE("WM - Core") {
	using namespace Gaze::WM;

	REQUIRE(IsInitialized() == false);
	REQUIRE(Init() == true);
	REQUIRE(IsInitialized() == true);
	Terminate();
	REQUIRE(IsInitialized() == false);
}
