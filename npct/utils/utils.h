#pragma once
#ifndef NPCT_UTILS_UTILS_H
#define NPCT_UTILS_UTILS_H

#include <string>
#include <vector>
#include <algorithm>
#include <cassert>

#ifdef _WIN32
#define NEWLINE "\r\n"
#elif defined macintosh // OS 9
#define NEWLINE "\r"
#else
#define NEWLINE "\n" // Mac OS X uses \n
#endif

namespace npct::utils {
    std::vector<std::string> split(const std::string& s, const std::string &delimeter);


    std::string generate_alnum_str(size_t len);
}

#endif //NPCT_UTILS_UTILS_H