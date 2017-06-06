//
// Created by lol on 6/6/2017.
//

#include "hex.h"

namespace npct::utils {

    std::string to_hex(const std::string& s, bool upper_case) noexcept
    {
        std::ostringstream ret;

        for (size_t i = 0; i < s.length(); ++i)
            ret << std::hex << std::setfill('0') << std::setw(2)
                << (upper_case ? std::uppercase : std::nouppercase) << (int)s[i];

        return ret.str();
    }

    std::string from_hex(const std::string &s) {
        std::istringstream in(s);
        std::ostringstream ret;

        std::string::value_type ch;
        for(size_t i = 0; i < s.length(); ++i) {
            in >> std::hex >> ch;
            ret << ch;
        }

        return ret.str();
    }

};
