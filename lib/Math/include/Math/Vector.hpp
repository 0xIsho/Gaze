#ifndef GAZE_MATH_VECTOR_HPP
#define GAZE_MATH_VECTOR_HPP

#include "Core/Type.hpp"

#include "Debug/Assert.hpp"

#include "Math/Util.hpp"

#include <cmath>
#include <string>
#include <utility>
#include <sstream>
#include <type_traits>

namespace Gaze::Math {
	/**
	 * @brief      Represents a Vector (in the mathematical sense)
	 *
	 * @tparam     T          The type of the components
	 * @tparam     N          The size of the Vector (aka The components count)
	 */
	template<typename T, U64 N>
		requires (N > 0)
	class Vector
	{
	public:
		/**
		 * The size of the vector (aka The components count).
		 */
		static constexpr auto kSize = N;

	public:
		/**
		 * @brief      Construct a new Vector object
		 *
		 * @param[in]  args  Initial component values
		 *
		 * @tparam     Args  Constructor arguments list
		 */
		template<typename...Args>
		constexpr Vector(Args... args) noexcept
			: m_Data{ static_cast<T>(std::forward<Args>(args))... }
		{
		}

		/**
		 * @brief      Subscript operator
		 *
		 * @param[in]  idx   The index
		 *
		 * @return     Reference to the `idx`th element.
		 *
		 * @note       `idx` *must* be `<N`
		 */
		[[nodiscard]] constexpr auto operator[](U64 idx) noexcept -> T&
		{
			GAZE_ASSERT(idx < N, "Index out of range");

			return m_Data[idx];
		}

#pragma region Convenience functions for different types of Vectors
		[[nodiscard]] constexpr auto X() const noexcept -> T
		{
			return m_Data[0];
		}

		constexpr auto SetX(T val) noexcept -> void
		{
			m_Data[0] = val;
		}

		template<typename U = T>
			requires(N >= 2 && std::is_convertible_v<T, U>)
		[[nodiscard]] constexpr auto Y() const noexcept -> T
		{
			return m_Data[1];
		}

		template<typename U = void>
			requires(N >= 2)
		constexpr auto SetY(T val) noexcept -> void
		{
			m_Data[1] = val;
		}

		template<typename U = T>
			requires(N >= 3 && std::is_convertible_v<T, U>)
		[[nodiscard]] constexpr auto Z() const noexcept -> T
		{
			return m_Data[2];
		}

		template<typename U = void>
			requires(N >= 3)
		constexpr auto SetZ(T val) noexcept -> void
		{
			m_Data[2] = val;
		}

		template<typename U = T>
			requires(N >= 4 && std::is_convertible_v<T, U>)
		[[nodiscard]] constexpr auto W() const noexcept -> T
		{
			return m_Data[3];
		}

		template<typename U = void>
			requires(N >= 4)
		constexpr auto SetW(T val) noexcept -> void
		{
			m_Data[3] = val;
		}

		[[nodiscard]] constexpr auto R() const noexcept -> T
		{
			return m_Data[0];
		}

		constexpr auto SetR(T val) noexcept -> void
		{
			m_Data[0] = val;
		}

		template<typename U = T>
			requires(N >= 2 && std::is_convertible_v<T, U>)
		[[nodiscard]] constexpr auto G() const noexcept -> T
		{
			return m_Data[1];
		}

		template<typename U = void>
			requires(N >= 2)
		constexpr auto SetG(T val) noexcept -> void
		{
			m_Data[1] = val;
		}

		template<typename U = T>
			requires(N >= 3 && std::is_convertible_v<T, U>)
		[[nodiscard]] constexpr auto B() const noexcept -> T
		{
			return m_Data[2];
		}

		template<typename U = void>
			requires(N >= 3)
		constexpr auto SetB(T val) noexcept -> void
		{
			m_Data[2] = val;
		}

		template<typename U = T>
			requires(N >= 4 && std::is_convertible_v<T, U>)
		[[nodiscard]] constexpr auto A() const noexcept -> T
		{
			return m_Data[3];
		}

		template<typename U = void>
			requires(N >= 4)
		constexpr auto SetA(T val) noexcept -> void
		{
			m_Data[4] = val;
		}
#pragma endregion

		/**
		 * @brief      Subscript operator (const version)
		 *
		 * @param[in]  idx   The index
		 *
		 * @return     The `idx`th element.
		 *
		 * @note       `idx` *must* be `<N`
		 */
		[[nodiscard]] constexpr auto operator[](U64 idx) const noexcept -> T
		{
			GAZE_ASSERT(idx < N, "Index out of range");

			return m_Data[idx];
		}

		/**
		 * @brief      Convert the Vector to a different Vector type
		 *
		 * This function converts the Vector to a different type. e.g.
		 * from `Vector<I32, 4>` to `Vector<F32, 4>`.
		 *
		 * @tparam     U     The new Vector's type
		 *
		 * @return     A new Vector of the specified type
		 *
		 * @note       This function does *not* do the conversion in place but
		 *             instead creates a new copy.
		 */
		template<typename U>
		[[nodiscard]] constexpr auto As() const noexcept -> Vector<U, N>
		{
			auto ret = Vector<U, N>{};

			for (auto i = U64(0); i < N; i++) {
				ret[i] = static_cast<U>(m_Data[i]);
			}

			return ret;
		}

		/**
		 * @brief      Get an iterator that points to the beginning of the
		 *             vector
		 *
		 * @return     An iterator that points to the beginning of the vector
		 */
		[[nodiscard]] constexpr auto begin()        noexcept { return std::begin(m_Data);  }
		/**
		 * @brief      Get an iterator that points to one past the end of the
		 *             vector
		 *
		 * @return     An iterator that points to the end of the vector
		 */
		[[nodiscard]] constexpr auto end()          noexcept { return std::end(m_Data);    }
		/**
		 * @brief      Get an iterator that points to the beginning of the
		 *             vector (const version)
		 *
		 * @return     An iterator that points to the beginning of the vector
		 */
		[[nodiscard]] constexpr auto begin()  const noexcept { return std::begin(m_Data);  }
		/**
		 * @brief      Get an iterator that points to the beginning of the
		 *             vector (const version)
		 *
		 * @return     An iterator that points to one past the end of the
		 *             vector
		 */
		[[nodiscard]] constexpr auto end()    const noexcept { return std::end(m_Data);    }
		/**
		 * @brief      Get a const iterator that points to the beginning of the
		 *             vector
		 *
		 * @return     A const iterator that points to the beginning of the vector
		 */
		[[nodiscard]] constexpr auto cbegin() const noexcept { return std::cbegin(m_Data); }
		/**
		 * @brief      Get a const iterator that points to the beginning of the
		 *             vector
		 *
		 * @return     A const iterator that points to one past the end of the
		 *             vector
		 */
		[[nodiscard]] constexpr auto cend()   const noexcept { return std::cend(m_Data);   }

	private:
		T m_Data[N]{}; /**< Components */
	};

	/**
	 * @brief      Calculates the Squared Magnitude of the Vector
	 *
	 * This function calculates the squared magnitude of the Vector. This is
	 * useful when, for example, comparing two vectors. This function should
	 * be used over Magnitude() whenever possible as this one doesn't envolve
	 * the quite slow `sqrt` operation.
	 *
	 * @param[in]  vec   The Vector
	 *
	 * @tparam     T     Vector type
	 * @tparam     N     Components count
	 *
	 * @return     The squared magnitude of the Vector
	 *
	 * Example Usage:
	 * @code
	 * const auto vec = Vector<F32, 3>(3.0F, 2.0F, 4.0F);
	 * const auto magSqrd = MagnitudeSquared(vec);
	 * @endcode
	 */
	template<typename T, U64 N>
	[[nodiscard]] constexpr auto MagnitudeSquared(const Vector<T, N>& vec) noexcept -> T
	{
		auto ret = T(0);

		for (auto elem : vec) {
			ret += elem * elem;
		}

		return ret;
	}

	/**
	 * @brief      Calculates the magnitude (length) of the Vector
	 *
	 * This function calculates the magnitude (length) of the Vector. Consider
	 * using MagnitudeSquared() if possible.
	 *
	 * @param[in]  vec   The Vector
	 *
	 * @tparam     T     Vector type
	 * @tparam     N     Components count
	 *
	 * @return     The magnitude of the Vector
	 *
	 * Example Usage:
	 * @code
	 * const auto vec = Vector<F32, 3>(2.0F, 3.0F, 4.0F);
	 * const auto mag = Magnitude(vec);
	 * @endcode
	 */
	template<typename T, U64 N>
	[[nodiscard]] constexpr auto Magnitude(const Vector<T, N>& vec) noexcept -> T
	{
		return std::sqrt(MagnitudeSquared(vec));
	}

	/**
	 * @brief      Normalize a Vector
	 *
	 * This function normalizes a Vector. A Normalized vector is a Vector with
	 * a Magnitude() of 1. Normalizing a Vector is done by dividing every
	 * component with the Vector's magnitude.
	 *
	 * @param[in]  vec   The Vector
	 *
	 * @tparam     T     Vector type
	 * @tparam     N     Components count
	 *
	 * @return     The normalized Vector
	 *
	 * Example Usage:
	 * @code
	 * const auto vec = Vector<F32, 3>(2.0F, 3.0F, 4.0F);
	 * const auto vecNorm = Normalize(vec);
	 * @endcode
	 */
	template<typename T, U64 N>
	[[nodiscard]] constexpr auto Normalize(Vector<T, N> vec) noexcept -> Vector<T, N>
	{
		const auto mag = Magnitude(vec);

		GAZE_ASSERT(mag != 0, "Vector Null/Zero cannot be normalized");

		for (auto i = U64(0); i < N; i++) {
			vec[i] /= mag;
		}

		return vec;
	}

	/**
	 * @brief      Calculate the Dot product of two Vectors
	 *
	 * This function calculates the Dot product of two Vectors.
	 *
	 * @param[in]  lhs   First Vector
	 * @param[in]  rhs   Second Vector
	 *
	 * @tparam     T     Vector type
	 * @tparam     N     Vector size
	 *
	 * @return     The Dot product of the Vectors
	 *
	 * Example Usage:
	 * @code
	 * const auto vec1 = Vector<F32, 3>(2, 4, 3);
	 * const auto vec2 = Vector<F32, 3>(6, 5, 8);
	 * const auto dot = Dot(vec1, vec2);
	 * @endcode
	 */
	template<typename T, U64 N>
	[[nodiscard]] constexpr auto Dot(Vector<T, N> lhs, Vector<T, N> rhs) noexcept -> T
	{
		auto ret = T(0);

		lhs = Normalize(std::move(lhs));
		rhs = Normalize(std::move(rhs));

		for (auto i = U64(0); i < N; i++) {
			ret += lhs[i] * rhs[i];
		}

		return ret;
	}

	/**
	 * @brief      Calculate the Cross product of two Vectors
	 *
	 * This function calculates the Cross product of two Vectors.
	 *
	 * @param[in]  lhs   The left hand side
	 * @param[in]  rhs   The right hand side
	 *
	 * @tparam     T     Vector type
	 *
	 * @return     The Cross product of the two Vectors
	 *
	 * Example Usage:
	 * @code
	 * const auto vec1 = Vector<F32, 3>(2, 4, 3);
	 * const auto vec2 = Vector<F32, 3>(6, 5, 8);
	 * const auto cross = Cross(vec1, vec2);
	 * @endcode
	 */
	template<typename T>
	[[nodiscard]] constexpr auto Cross(Vector<T, 3> lhs, const Vector<T, 3>& rhs) noexcept -> Vector<T, 3>
	{
		// Indices of the respective components
		constexpr auto x = U64(0);
		constexpr auto y = U64(1);
		constexpr auto z = U64(2);

		return {
			lhs[y] * rhs[z] - lhs[z] * rhs[y],
			lhs[z] * rhs[x] - lhs[x] * rhs[z],
			lhs[x] * rhs[y] - lhs[y] * rhs[x]
		};
	}

	/**
	 * @brief        Add two Vectors
	 *
	 * @param[inout] lhs   The left hand side
	 * @param[in]    rhs   The right hand side
	 *
	 * @tparam       T     Vector type
	 * @tparam       N     Component count
	 *
	 * @return     Reference to the left-hand side
	 */
	template<typename T, U64 N>
	constexpr auto operator+=(Vector<T, N>& lhs, const Vector<T, N>& rhs) noexcept -> Vector<T, N>&
	{
		for (auto i = U64(0); i < N; i++) {
			lhs[i] += rhs[i];
		}

		return lhs;
	}

	/**
	 * @brief      Add two Vectors
	 *
	 * @param[in]  lhs   The left hand side
	 * @param[in]  rhs   The right hand side
	 *
	 * @tparam     T     Vector type
	 * @tparam     N     Components count
	 *
	 * @return     The result of the addition as a new Vector
	 */
	template<typename T, U64 N>
	[[nodiscard]] constexpr auto operator+(Vector<T, N> lhs, const Vector<T, N>& rhs) noexcept -> Vector<T, N>
	{
		return lhs += rhs;
	}

	/**
	 * @brief        Subtract two Vectors
	 *
	 * @param[inout] lhs   The left hand side
	 * @param[in]    rhs   The right hand side
	 *
	 * @tparam       T     Vector type
	 * @tparam       N     Component count
	 *
	 * @return     Reference to the left-hand side
	 */
	template<typename T, U64 N>
	constexpr auto operator-=(Vector<T, N>& lhs, const Vector<T, N>& rhs) noexcept -> Vector<T, N>&
	{
		for (auto i = U64(0); i < N; i++) {
			lhs[i] -= rhs[i];
		}

		return lhs;
	}

	/**
	 * @brief      Subtract two Vectors
	 *
	 * @param[in]  lhs   The left hand side
	 * @param[in]  rhs   The right hand side
	 *
	 * @tparam     T     Vector type
	 * @tparam     N     Components count
	 *
	 * @return     The result of the subtraction as a new Vector
	 */
	template<typename T, U64 N>
	[[nodiscard]] constexpr auto operator-(Vector<T, N> lhs, const Vector<T, N>& rhs) noexcept -> Vector<T, N>
	{
		return lhs -= rhs;
	}

	/**
	 * @brief        Scale a Vector
	 *
	 * This function multiplies every component of the Vector with the provided
	 * `scalar`.
	 *
	 * @param[inout] lhs   The left hand side
	 * @param[in]    rhs   The right hand side
	 *
	 * @tparam       T     Vector type
	 * @tparam       N     Component count
	 * @tparam       U     Scalar type
	 *
	 * @return     Reference to the left-hand side
	 */
	template<typename T, U64 N, typename U>
	constexpr auto operator*=(Vector<T, N>& lhs, U scalar) noexcept -> Vector<T, N>&
	{
		for (auto& elem : lhs) {
			elem *= scalar;
		}

		return lhs;
	}

	/**
	 * @brief      Scale a Vector
	 *
	 * This function multiplies every component of the Vector with the provided
	 * `scalar`.
	 *
	 * @param[in]  lhs   The left hand side
	 * @param[in]  rhs   The right hand side
	 *
	 * @tparam     T     Vector type
	 * @tparam     N     Components count
	 * @tparam       U     Scalar type
	 *
	 * @return     The result of the multiplication as a new Vector
	 */
	template<typename T, U64 N, typename U>
	[[nodiscard]] constexpr auto operator*(Vector<T, N> lhs, U scalar) noexcept -> Vector<T, N>
	{
		return lhs *= scalar;
	}

	/**
	 * @brief      Scale a Vector
	 *
	 * This function divides every component of the Vector with the provided
	 * `scalar`.
	 *
	 * @param[in]  lhs   The left hand side
	 * @param[in]  rhs   The right hand side
	 *
	 * @tparam     T     Vector type
	 * @tparam     N     Components count
	 *
	 * @return     Reference to the left-hand side
	 */
	template<typename T, U64 N>
	constexpr auto operator/=(Vector<T, N>& lhs, U64 scalar) noexcept -> Vector<T, N>&
	{
		for (auto& elem : lhs) {
			elem /= scalar;
		}

		return lhs;
	}

	/**
	 * @brief      Scale a Vector
	 *
	 * This function divides every component of the Vector with the provided
	 * `scalar`.
	 *
	 * @param[in]  lhs   The left hand side
	 * @param[in]  rhs   The right hand side
	 *
	 * @tparam     T     Vector type
	 * @tparam     N     Components count
	 *
	 * @return     The result of the division as a new Vector
	 */
	template<typename T, U64 N>
	[[nodiscard]] constexpr auto operator/(Vector<T, N> lhs, U64 scalar) noexcept -> Vector<T, N>
	{
		return lhs /= scalar;
	}

	/**
	 * @brief      Compare two Vectors for equality
	 *
	 * @param[in]  lhs  Left-hand side
	 * @param[in]  rhs  Right-hand side
	 *
	 * @tparam     T          Vector type
	 * @tparam     N          Component count
	 *
	 * @return     `true` if the Vectors are equal. `false` otherwise.
	 *
	 * @note       This function doesn't work for floating-point Vector types.
	 *             Use ApproxEq(const Vector&, const Vector&, T) for comparing
	 *             Vectors of floating-point types.
	 */
	template<typename T, U64 N>
		requires (!std::is_floating_point_v<T>)
	[[nodiscard]] constexpr auto operator==(const Vector<T, N>& lhs, const Vector<T, N>& rhs) noexcept -> bool
	{
		for (auto i = U64(0); i < N; i++) {
			if (lhs[i] != rhs[i]) {
				return false;
			}
		}
		return true;
	}

	/**
	 * @brief      Compare two Vectors for inequality
	 *
	 * @param[in]  lhs  Left-hand side
	 * @param[in]  rhs  Right-hand side
	 *
	 * @tparam     T          Vector type
	 * @tparam     N          Component count
	 *
	 * @return     `true` if the Vectors are not equal. `false` otherwise.
	 *
	 * @note       This function doesn't work for floating-point Vector types.
	 *             Use ApproxEq(const Vector&, const Vector&, T) for comparing
	 *             Vectors of floating-point types.
	 */
	template<typename T, U64 N>
		requires (!std::is_floating_point_v<T>)
	[[nodiscard]] constexpr auto operator!=(const Vector<T, N>& lhs, const Vector<T, N>& rhs) noexcept -> bool
	{
		return !(lhs == rhs);
	}

	/**
	 * @brief      Compare two Vectors for equality
	 *
	 * This function compares two Vectors of floating-point types for
	 * approximate equality.
	 *
	 * @param[in]  lhs        Left-hand side
	 * @param[in]  rhs        Right-hand side
	 * @param[in]  tolerance  Acceptable margin of error
	 *
	 * @tparam     T          Vector type
	 * @tparam     N          Component count
	 *
	 * @return     `true` if the difference between the Vectors' components is
	 *             within the tolerable margin of error. `false` otherwise.
	 *
	 * Example Usage:
	 * @code
	 * const auto vec1 = Vector<F32, 4>(1, 2, 4, 2);
	 * const auto vec2 = Vector<F32, 4>(1, 3, 5, 7);
	 *
	 * if (ApproxEq(vec1, vec2)) {
	 *   // vec1 == vec2
	 * } else {
	 *   // vec1 != vec2
	 * }
	 * @endcode
	 */
	template<typename T, U64 N>
		requires(std::is_floating_point_v<T>)
	[[nodiscard]] constexpr auto ApproxEq(
		const Vector<T, N>& lhs,
		const Vector<T, N>& rhs,
		T tolerance = DefaultTolerance<T>::Value
	) noexcept -> bool
	{
		for (auto i = U64(0); i < N; i++) {
			if (!ApproxEq(lhs[i], rhs[i], tolerance)) {
				return false;
			}
		}

		return true;
	}

	template<typename T, U64 N, typename U = F32>
	[[nodiscard]] constexpr auto Lerp(const Vector<T, N>& a, const Vector<T, N>& b, U t) noexcept -> Vector<T, N>
	{
		auto ret = Vector<T, N>();

		for (auto i = U64(0); i < N; i++) {
			ret[i] = Lerp(a[i], b[i], t);
		}

		return ret;
	}
}

namespace Gaze {
	template<typename T, U64 N>
	[[nodiscard]] auto ToString(const Math::Vector<T, N>& vec) -> std::string
	{
		auto stream = std::ostringstream();

		stream << '[' << vec[0];
		for (auto i = U64(1); i < N; i++) {
			stream << ',' << vec[i];
		}
		stream << ']';

		return stream.str();
	}
}

#endif // GAZE_MATH_VECTOR_HPP
