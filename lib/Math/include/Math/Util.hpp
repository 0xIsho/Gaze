#ifndef GAZE_MATH_UTIL_HPP
#define GAZE_MATH_UTIL_HPP

#include "Core/Type.hpp"

#include <cmath>
#include <type_traits>

namespace Gaze::Math {
	/**
	 * @brief      Default tolerance trait for approximate equality tests
	 *
	 * @tparam     T     The floating point type
	 */
	template<typename T>
		requires (std::is_floating_point_v<T>)
	struct DefaultTolerance;

	/**
	 * @brief      Default tolerance trait for `F32`
	 */
	template<>
	struct DefaultTolerance<F32>
	{
		static constexpr auto Value = F32(1e-6);
	};

	/**
	 * @brief      Default tolerance trait for `F64`
	 */
	template<>
	struct DefaultTolerance<F64>
	{
		static constexpr auto Value = F64(1e-9);
	};

	/**
	 * @brief      Compare two floating point values for approximate equality
	 *
	 * This function compares two floating point values for equality with some
	 * tolerance for precision issues.
	 *
	 * @param[in]  x          First value
	 * @param[in]  y          Second value
	 * @param[in]  tolerance  The tolerance
	 *
	 * @return     `true` if the difference between the 2 values is within the
	 *             margin of error, `false` otherwise.
	 */
	template<typename T>
		requires (std::is_floating_point_v<T>)
	[[nodiscard]] constexpr auto ApproxEq(T x, T y, T tolerance = DefaultTolerance<T>::Value) noexcept -> bool {
		return std::fabs(x - y) <= tolerance;
	}

	template<typename T, typename U = F32>
	[[nodiscard]] constexpr auto Lerp(T a, T b, U t) noexcept -> T {
		return std::lerp(a, b, t);
	}
}

#endif //  GAZE_MATH_UTIL_HPP
