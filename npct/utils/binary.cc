//
// Created by lol on 6/6/2017.
//

#include "binary.h"

#include <bitset>
#include <sstream>

namespace npct::utils {

    template<size_t n>
    std::bitset<n> reverse(std::bitset<n> &set)
    {
        std::bitset<n> result;

        for (size_t i = 0; i < n; i++)
            result[i] = set[n - i - 1];

        return result;
    }

    std::string to_bin(const std::string &s, const std::string &delim, bool msb_end) {
        std::ostringstream os;
        using bs = std::bitset<CHAR_BIT*sizeof(std::string::value_type)>;
        for (size_t i = 0; i < s.size(); ++i)
        {
            auto b = bs((unsigned long) s[i]);
            if (msb_end) b = reverse(b);
            os << b << delim;
        }
        return os.str();
    }

}