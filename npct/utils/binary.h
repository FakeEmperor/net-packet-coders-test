//
// Created by lol on 6/6/2017.
//

#ifndef NPCT_UTILS_BINARY_H
#define NPCT_UTILS_BINARY_H

#include <string>

namespace npct::utils {

    std::string to_bin(const std::string &s, const std::string &delim = "", bool msb_end = true);

}

#endif //NPCT_UTILS_BINARY_H
