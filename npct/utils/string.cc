#include "string.h"

namespace npct::utils::string
{
    bool __ascii_icmp_pred(unsigned char a, unsigned char b)
    {
        return std::tolower(a) == std::tolower(b);
    }
}