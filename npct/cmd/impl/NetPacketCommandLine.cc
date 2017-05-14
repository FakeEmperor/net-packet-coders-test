//
// Created by Raaw on 5/14/2017.
//

#include <iostream>
#include <list>
#include <cstring>
#include <climits>
#include "NetPacketCommandLine.h"

namespace npct::cmd::impl {


////////

    const std::string CommandLine::HELP_TEXT =
            "Usage <mode> [parameters...]\r\n\
Modes available:\r\n\
  -g - generates 'num_sources'*|'noise_levels'| datasets for an encoding algorithm in \"encode\" mode.\r\n\
\t Parameters are: -max_source_size <int> -noise_levels <array[float][0..1]> -num_sources <int> -io_sources <str>\r\n\
\t Program generates a new dataset for \"encoding\" mode. For each noise level num_sources of tests will be generated.\r\n\
  -s - generates dataset for an encoding algorithm in \"decode\" mode.\r\n\
\t Parameters are: -io_noised <str> -in_encoded <str> -io_sources <str> \r\n\
\t Program generates a new dataset for \"decoding\" mode using coder's output and source data.\r\n\
  -d - evaluates the accuracy and other parameters of both encoding and decoding algorithms.\r\n\
\t Parameters are: -in_encoded <str> -in_decoded <str> -io_noised <str> -io_source <str> -out_report <str> \r\n\
\t Program performs accuracy test, evaluates speed and other algorithm's parameters and writes them into 'out_report' file. \r\n\
\r\n\
Note: data type '<array[type]>' is a coma (',') delimeted list of values of given type. \
E.g.: <array[float[0..1]]> - 0,0.2,0.4,0.8,0.9,1\r\n\
";


    const std::string
            CommandLine::Values::PARAM_NOISE_LEVELS = "noise_levels",
            CommandLine::Values::PARAM_SOURCE_MAXSIZE = "max_source_size",
            CommandLine::Values::PARAM_NUM_SOURCES = "num_sources",
//CommandLine::Values::PARAM_DIFFICULTIES = "difficulties",

            CommandLine::Values::INPUT_ENCODED_DATA = "in_encoded",
            CommandLine::Values::INPUT_DECODED_DATA = "in_decoded",
            CommandLine::Values::INOUT_NOISED_DATA = "io_noised",
            CommandLine::Values::OUTPUT_REPORT = "out_report",
            CommandLine::Values::INOUT_SOURCE_DATA = "io_source";

    const std::string
            CommandLine::Flags::MODE_CHECK_DECODE = "d",
            CommandLine::Flags::MODE_SEND_DATA = "s",
            CommandLine::Flags::MODE_GENERATE_DATA = "g";


    const std::set<std::string>
            CommandLine::VALUED_OPTS = {
            Values::PARAM_NOISE_LEVELS,
            //Values::PARAM_DIFFICULTIES ,
            Values::PARAM_SOURCE_MAXSIZE,
            Values::PARAM_NUM_SOURCES,

            Values::INPUT_ENCODED_DATA,
            Values::INOUT_NOISED_DATA,
            Values::INPUT_DECODED_DATA,
            Values::OUTPUT_REPORT,
            Values::INOUT_SOURCE_DATA
    },
            CommandLine::FLAG_OPTS = {
            Flags::MODE_CHECK_DECODE,
            Flags::MODE_SEND_DATA,
            Flags::MODE_GENERATE_DATA
    };

    template<const std::string &...modes>
    std::pair<bool, std::string> __check_must_be_in(const CommandLine &cmd, const std::string &name, bool has) {
        std::string err_msg = "'" + name + "' must be set";
        bool has_mode = false;

        for (const auto &m : {modes...}) {
            has_mode = cmd.getFlagVal(m);
            if (has_mode) break;
        }
        return std::make_pair<bool, std::string>(!has_mode || has_mode && has, std::move(err_msg));
    };

    auto __check_mode(const CommandLine &cmd, const std::string &name, bool has) {
        static const auto &err_msg = "Either one option must be selected: -" + CommandLine::Flags::MODE_SEND_DATA +
                                     ", -" + CommandLine::Flags::MODE_GENERATE_DATA +
                                     " or -" + CommandLine::Flags::MODE_CHECK_DECODE;
        return std::make_pair<bool, std::string>(
                cmd.isOptSet(CommandLine::Flags::MODE_SEND_DATA) +
                cmd.isOptSet(CommandLine::Flags::MODE_CHECK_DECODE) +
                cmd.isOptSet(CommandLine::Flags::MODE_GENERATE_DATA) == 1,
                std::string(err_msg));
    };

    auto __check_noise_dif(const CommandLine &cmd, const std::string &name, bool has) {
        static const auto &err_msg = "Either one option must be selected: -" +
                                     /*CommandLine::Values::PARAM_DIFFICULTIES + " or -" + */CommandLine::Values::PARAM_NOISE_LEVELS;
        auto check_result = 0;
        auto mode_ok = __check_must_be_in<CommandLine::Flags::MODE_GENERATE_DATA>(cmd, name, has);
        if (!mode_ok.first)
            return mode_ok;

        return std::make_pair<bool, std::string>(
                /*!cmd.isOptSet(CommandLine::Values::PARAM_DIFFICULTIES)*/ true,
                                                                           std::string(err_msg));
    };

    template<int min, int max>
    std::pair<bool, std::string>
    __value_check_int_range(const CommandLine &cmd, const std::string &name, const std::string &val) {
        auto parsed = CommandProcessor::Parse<int>(val);
        return std::make_pair<bool, std::string>(parsed >= min && parsed <= max,
                                                 "Value of '-" + name + "' must lie in intervals [" +
                                                 std::to_string(min) + ", " + std::to_string(max) + "]");
    }

    const std::map<std::string, CommandLine::ValidatorEntry> CommandLine::VALIDATORS = {
            {Flags::MODE_CHECK_DECODE,     {__check_mode, nullptr}},

            {Flags::MODE_SEND_DATA,        {__check_mode, nullptr}},
            {Flags::MODE_GENERATE_DATA,    {__check_mode, nullptr}},
            {Values::PARAM_NOISE_LEVELS,   {__check_noise_dif,
                                            [](const CommandLine &cmd, const std::string &name, const std::string &val)
                                               {
                                                   auto parsed = Parse<std::vector<float>>(val);
                                                   bool success = true;
                                                   for (auto p : parsed)
                                                       if (p < 0 || p > 1 || fabs(p - 0.5f) < 0.2) {
                                                           success = false;
                                                           break;
                                                       }
                                                   return std::make_pair<bool, std::string>(std::move(success),
                                                                                            "Noise must lie in [0, 1] interval and must not be close to a 0.5 value");
                                               }
                                           }
            },
            {Values::INOUT_NOISED_DATA,    {__check_must_be_in<Flags::MODE_CHECK_DECODE, Flags::MODE_SEND_DATA>, nullptr}},
            {Values::INPUT_ENCODED_DATA,   {__check_must_be_in<Flags::MODE_SEND_DATA, Flags::MODE_CHECK_DECODE>, nullptr}},
            {Values::INPUT_DECODED_DATA,   {__check_must_be_in<Flags::MODE_CHECK_DECODE>, nullptr}},
            {Values::INOUT_SOURCE_DATA,    {__check_must_be_in<Flags::MODE_GENERATE_DATA, Flags::MODE_SEND_DATA, Flags::MODE_CHECK_DECODE>, nullptr}},
            {Values::OUTPUT_REPORT,        {__check_must_be_in<Flags::MODE_CHECK_DECODE>, nullptr}},
            {Values::PARAM_SOURCE_MAXSIZE, {__check_must_be_in<Flags::MODE_GENERATE_DATA>, __value_check_int_range<1, 4'000'000>}},
            {Values::PARAM_NUM_SOURCES,    {__check_must_be_in<Flags::MODE_GENERATE_DATA>, __value_check_int_range<1, INT_MAX>}},/**/
    };
// END OF VALIDATORS

};