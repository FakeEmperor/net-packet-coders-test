//
// Created by lol on 6/6/2017.
//

#ifndef NPCT_UTILS_HEX_H
#define NPCT_UTILS_HEX_H

#include <sstream>
#include <iomanip>
#include <string>

namespace npct::utils {

    std::string to_hex(const std::string& s, bool upper_case = true ) noexcept;
    std::string from_hex(const std::string &s);

};


#endif //NPCT_UTILS_HEX_H
