#include "portable_endian.h"    // __BYTE_ORDER
#include <algorithm>   // std::reverse

namespace npct::net::utils
{
    template <typename T>
    constexpr T hton(T value) noexcept
    {
#if __BYTE_ORDER == __LITTLE_ENDIAN
        char* ptr = reinterpret_cast<char*>(&value);
        std::reverse(ptr, ptr + sizeof(T));
#endif
        return value;
    }

    template <typename T>
    constexpr T ntoh(T value) noexcept
    {
#if __BYTE_ORDER == __LITTLE_ENDIAN
        char* ptr = reinterpret_cast<char*>(&value);
        std::reverse(ptr, ptr + sizeof(T));
#endif
        return value;
    }
}
