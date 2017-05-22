//
// Created by Raaw on 5/17/2017.
//

#ifndef NPCT_CMD_CMD_TCC
#define NPCT_CMD_CMD_TCC
#include "cmd.h"
////////////   COMMAND_PROCESSOR

namespace npct::cmd {

    // well, no if-constexpr and return/auto in MSVC, touche
    template <>
    inline std::string CommandProcessor::Parse<std::string>(const std::string& val)
    {
        return val;
    }

    template <>
    inline int CommandProcessor::Parse<int>(const std::string& val) noexcept(false)
    {
        return std::stoi(val);
    }

    template <>
    inline float CommandProcessor::Parse<float>(const std::string& val) noexcept(false)
    {
        return std::stof(val);
    }

    template <>
    inline bool CommandProcessor::Parse<bool>(const std::string& val) noexcept(false)
    {
        auto icompare = [](const unsigned char a, const unsigned char b) -> bool
        {
            return tolower(a) == tolower(b);
        };
        if (std::equal(val.begin(), val.end(), std::begin("false")))
            return false;
        if (std::equal(val.begin(), val.end(), std::begin("true")))
            return true;
        throw std::runtime_error("Conversion to bool failed");
    }

};
#endif //NPCT_CMD_CMD_TCC
