#ifndef GAZE_CORE_TYPES_HPP
#define GAZE_CORE_TYPES_HPP

#include <memory>
#include <cstdint>

namespace Gaze {
	/**
	 * Unsigned 8-bit integer.
	 */
	using U8  = std::uint8_t;
	/**
	 * Unsigned 16-bit integer.
	 */
	using U16 = std::uint16_t;
	/**
	 * Unsigned 32-bit integer.
	 */
	using U32 = std::uint32_t;
	/**
	 * Unsigned 64-bit integer.
	 */
	using U64 = std::uint64_t;

	/**
	 * Signed 8-bit integer.
	 */
	using I8  = std::int8_t;
	/**
	 * Signed 16-bit integer.
	 */
	using I16 = std::int16_t;
	/**
	 * Signed 32-bit integer.
	 */
	using I32 = std::int32_t;
	/**
	 * Signed 64-bit integer.
	 */
	using I64 = std::int64_t;

	/**
	 * Integer type that may hold a pointer
	 */
	using IPtr = I64;

	/**
	 * 32-bit floating point number.
	 */
	using F32 = float;
	/**
	 * 64-bit floating point number.
	 */
	using F64 = double;

	/**
	 * Unsigned byte
	 */
	using Byte = U8;

	template<typename T>
	struct Tag {};

	/**
	 * @brief POD wrapper
	 *
	 * This template is meant to disambiguate function parameters and disable
	 * implicit conversion for said type.
	 *
	 * An example use can be found in the OpenGL's Vertex Array implementation
	 * in `GFX/Platform/OpenGL/Objects/VertexArray.hpp`.
	 *
	 * @tparam T Type to wrap
	 * @tparam TagType Tag type to use for disambiguation
	 */
	template<typename T, typename TagType = Tag<T>>
	class ValueWrapper
	{
	public:
		constexpr explicit ValueWrapper(T&& val) noexcept
			: m_Value(std::forward<T>(val))
		{
		}

		constexpr auto Value() const noexcept -> T
		{
			return m_Value;
		}

		constexpr explicit operator T() const noexcept
		{
			return m_Value;
		}

	private:
		T m_Value;
	};

	namespace Mem {
		/**
		 * Unique pointer type
		 */
		template<typename T>
		using Unique = std::unique_ptr<T>;

		/**
		 * Shared pointer type
		 */
		template<typename T>
		using Shared = std::shared_ptr<T>;

		/**
		 * @brief Create a unique pointer
		 *
		 * @tparam T Type of the pointer
		 * @tparam Args Types of the arguments
		 * @param args Arguments to forward to the constructor of T
		 *
		 * @return Unique pointer to T
		 */
		template<typename T, typename... Args>
		auto MakeUnique(Args&&... args)
		{
			return std::make_unique<T>(std::forward<Args>(args)...);
		}

		/**
		 * @brief Create a shared pointer
		 *
		 * @tparam T Type of the pointer
		 * @tparam Args Types of the arguments
		 * @param args Arguments to forward to the constructor of T
		 *
		 * @return Shared pointer to T
		 */
		template<typename T, typename... Args>
		auto MakeShared(Args&&... args)
		{
			return std::make_shared<T>(std::forward<Args>(args)...);
		}
	}
}

#endif // GAZE_CORE_TYPES_HPP
