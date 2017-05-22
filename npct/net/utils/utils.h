#include "portable_endian.h"    // __BYTE_ORDER
#include <algorithm>   // std::reverse

namespace npct::net::utils
{
#if __cplusplus >= 201402L
	template <typename T>
	constexpr T hreverse(T value) noexcept
	{
#if __BYTE_ORDER == __LITTLE_ENDIAN
		auto* ptr = reinterpret_cast<char*>(&value);
		std::reverse(ptr, ptr + sizeof(T));
#endif
		return value;
	}
#else
	template <typename T>
	constexpr T hreverse(T value, char* ptr = 0) noexcept
	{
		return
#if __BYTE_ORDER == __LITTLE_ENDIAN
			ptr = reinterpret_cast<char*>(&value),
			std::reverse(ptr, ptr + sizeof(T)),
#endif
			value;
	}
#endif

	template <typename T>
	constexpr T hton(T value) noexcept
	{
		return hreverse(value);
	}

    

    template <typename T>
    constexpr T ntoh(T value) noexcept
    {
		return hreverse(value);
    }



}
