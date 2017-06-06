#ifndef NPCT_UTILS_STRING_H
#define NPCT_UTILS_STRING_H

#include <cstring>
#include <cctype>
#include <algorithm>


namespace npct::utils::string
{


    bool __ascii_icmp_pred(unsigned char a, unsigned char b);


    template <typename Tx, typename Ty>
    bool ascii_icmp(const Tx &x, const Ty &y)
    {
        if (x.length() != y.length())
            return false;
        return std::equal(std::begin(x), std::end(x), std::begin(y), __ascii_icmp_pred);

    }




}

#endif // !NPCT_UTILS_STRING_H
