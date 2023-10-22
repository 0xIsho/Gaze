#include "Math/Util.hpp"
#include "Math/Vector.hpp"

#include <catch2/catch.hpp>

#include <cmath>

using namespace Gaze;
using namespace Gaze::Math;

TEST_CASE("Vector") {
	SECTION("Basics") {
		REQUIRE(Vector<int, 4>::kSize == 4);
	}
	SECTION("Initialization and Access") {
		auto vec = Vector<int, 3>{ 3, 2, 4 };
		REQUIRE(vec.kSize == 3);

		constexpr auto vecZero = Vector<int, 2>{};
		REQUIRE(vecZero == Vector<int, 2>{ 0, 0 });

		// Check the const and non-const operator[]
		SECTION("Non-const access") {
			REQUIRE(vec[0] == 3);
			REQUIRE(vec[1] == 2);
			REQUIRE(vec[2] == 4);

			vec[0] += 1;
			REQUIRE(vec[0] == 4);
		}
		SECTION("Const access") {
			const auto& constVec = vec;
			REQUIRE(constVec[0] == 3);
			REQUIRE(constVec[1] == 2);
			REQUIRE(constVec[2] == 4);
		}
	}
	SECTION("Iterators") {
		auto vec = Vector<int, 4>{ 1, 2 };

		auto begIt = vec.begin();
		auto endIt = vec.end();

		REQUIRE(*(begIt + 0) == 1);
		REQUIRE(*(begIt + 1) == 2);
		REQUIRE(endIt == begIt + vec.kSize);
	}
	SECTION("Operators") {
		auto vec1 = Vector<int, 4>{ 1, 2, 3, 4 };
		auto vec2 = Vector<int, 4>{ 2, 3, 4, 5 };

		SECTION("Equality") {
			auto vecEq = Vector<int, 4>{ 1, 2, 3, 4 };

			REQUIRE(vec1 == vecEq);
			REQUIRE(vec1 != vec2);
		}

		SECTION("Addition") {
			REQUIRE(vec1 + vec2 == Vector<int, 4>{ 3, 5, 7, 9 });
		}

		SECTION("Compound Addition") {
			vec1 += vec2;

			REQUIRE(vec1 == Vector<int, 4>{ 3, 5, 7, 9 });
		}

		SECTION("Subtraction") {
			REQUIRE(vec1 - vec2 == Vector<int, 4>{ -1, -1, -1, -1 });
		}

		SECTION("Compound Subtraction") {
			vec1 -= vec2;

			REQUIRE(vec1 == Vector<int, 4>{ -1, -1, -1, -1 });
		}

		SECTION("Scalar Multiplication") {
			REQUIRE(vec1 * 2 == Vector<int, 4>{ 2, 4, 6, 8 });
		}

		SECTION("Compound Scalar Multiplication") {
			vec1 *= 2;

			REQUIRE(vec1 == Vector<int, 4>{ 2, 4, 6, 8 });
		}

		SECTION("Scalar Division") {
			const auto floatVec = vec2.As<F32>();

			REQUIRE(vec2 / 2 == Vector<int, 4>{ 1, 1/*.5*/, 2, 2/*.5*/ });
			REQUIRE(ApproxEq(floatVec / 2, Vector<F32, 4>{ 1.0F, 1.5F, 2.0F, 2.5F }));
		}

		SECTION("Compound Scalar Division") {
			auto floatVec2 = vec2.As<F32>();

			vec2 /= 2;
			floatVec2 /= 2;

			REQUIRE(vec2 == Vector<int, 4>{ 1, 1/*.5*/, 2, 2/*.5*/ });
			REQUIRE(ApproxEq(floatVec2, Vector<F32, 4>{ 1.0F, 1.5F, 2.0F, 2.5F }));
		}

		SECTION("Stringify") {
			REQUIRE(ToString(vec1) == "[1,2,3,4]");
		}
	}
	SECTION("Vector Calculations") {
		SECTION("Magnitude/Length") {
			const auto vec = Vector<F32, 2>(4, 2);
			constexpr auto kExpected = 4.472135955F;

			REQUIRE(ApproxEq(Magnitude(vec), kExpected));
			REQUIRE(ApproxEq(MagnitudeSquared(vec), std::pow(kExpected, 2.0F)));
		}
		SECTION("Normalization") {
			const auto vec = Normalize(Vector<F32, 2>(4, 2));

			REQUIRE(ApproxEq(Magnitude(vec), 1.0F));
			REQUIRE(ApproxEq(MagnitudeSquared(vec), 1.0F));
		}
		SECTION("Dot Product") {
			const auto vec1 = Vector<F32, 3>(0.6, -0.8, 0);
			const auto vec2 = Vector<F32, 3>(0, 1, 0);

			REQUIRE(ApproxEq(Dot(vec1, vec2), -0.8F));
		}
		SECTION("Cross Product") {
			const auto vec1 = Vector<F32, 3>(2, 3, 4);
			const auto vec2 = Vector<F32, 3>(1, 5, 6);
			const auto expected = Vector<F32, 3>(-2, -8, 7);

			REQUIRE(ApproxEq(Cross(vec1, vec2), expected));
		}
	}
}
