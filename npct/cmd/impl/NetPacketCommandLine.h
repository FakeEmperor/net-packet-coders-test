//
// Created by Raaw on 5/14/2017.
//

#ifndef NPCT_CMD_IMPL_NETPACKET_COMMANDLINE_H
#define NPCT_CMD_IMPL_NETPACKET_COMMANDLINE_H

#include <cmd/cmd.h>

namespace npct::cmd::impl {
    class CommandLine : protected CommandProcessor
    {
    private:
        enum class Mode { SEND, CHECK, GENERATE, UNKNOWN };
        Mode mode_ = { Mode::UNKNOWN };
    protected:

        void parseArgs_(const char** argv, int argc) noexcept(false);

    public:
        static const std::string HELP_TEXT;

        struct Flags {
            static const std::string MODE_CHECK_DECODE, MODE_SEND_DATA, MODE_GENERATE_DATA;
        };
        struct Values
        {
            static const std::string
                    PARAM_SOURCE_MAXSIZE,
                    PARAM_NUM_SOURCES,
                    PARAM_NOISE_LEVELS,

                    INPUT_ENCODED_DATA,
                    INPUT_DECODED_DATA,
                    INOUT_NOISED_DATA,
                    OUTPUT_REPORT,
                    INOUT_SOURCE_DATA;

        };

        template <typename Arg>
        using ValidatorFn = std::function<std::pair<bool, std::string>(const CommandLine &, const std::string &, Arg)> ;

        // { <PRESENCE VALIDATION/NULLPTR (HAS/OR NOT) -> { VALID/INVALID, ERR_MSG } >, <VALIDATING_FUNCTION/NULLPTR -> { VALID/INVALID, ERR_MSG }>  }
        typedef std::pair<ValidatorFn<bool>, ValidatorFn<const std::string &>> ValidatorEntry;

        static const std::set<std::string> VALUED_OPTS, FLAG_OPTS;

        static const std::map<std::string, ValidatorEntry> VALIDATORS;

        CommandLine();


        void start(const char** argv, size_t argc);


        using CommandProcessor::isOptFlag;
        using CommandProcessor::getFlagVal;
        using CommandProcessor::isOptSet;
        using CommandProcessor::getOpt;
        using CommandProcessor::setOpt;

        using CommandProcessor::Parse;
    };
}




#endif //NPCT_CMD_IMPL_NETPACKET_COMMANDLINE_H
