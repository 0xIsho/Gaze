#ifndef GAZE_CORE_TYPES_HPP
#define GAZE_CORE_TYPES_HPP

#include <memory>
#include <cstdint>

namespace Gaze {
	using U8 = std::uint8_t;
	using U16 = std::uint16_t;
	using U32 = std::uint32_t;
	using U64 = std::uint64_t;

	using I8 = std::int8_t;
	using I16 = std::int16_t;
	using I32 = std::int32_t;
	using I64 = std::int64_t;

	using IPtr = I64;

	using F32 = float;
	using F64 = double;

	using Byte = U8;

	namespace Mem {
		template<typename T>
		using Unique = std::unique_ptr<T>;

		template<typename T>
		using Shared = std::shared_ptr<T>;

		template<typename T, typename... Args>
		auto MakeUnique(Args&&... args)
		{
			return std::make_unique<T>(std::forward<Args>(args)...);
		}

		template<typename T, typename... Args>
		auto MakeShared(Args&&... args)
		{
			return std::make_shared<T>(std::forward<Args>(args)...);
		}
	}
}

#endif // GAZE_CORE_TYPES_HPP
