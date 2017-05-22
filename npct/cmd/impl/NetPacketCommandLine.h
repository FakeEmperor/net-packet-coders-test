//
// Created by Raaw on 5/14/2017.
//

#ifndef NPCT_CMD_IMPL_NETPACKET_COMMANDLINE_H
#define NPCT_CMD_IMPL_NETPACKET_COMMANDLINE_H

#include <cmd/cmd.h>
#include "NetPacketMode.h"
#include <utils/string_literal.h>

namespace npct::cmd::impl {

	

	class NetPacketCommandLine final : protected CommandLine < NetPacketMode, void, utils::string_literal < 'm', 'o', 'd', 'e' >>
    {
    public:
        ///////////////  SUPPORT CLASSES  ///////////////
        typedef cmd::CommandLine<NetPacketMode, void, utils::string_literal < 'm', 'o', 'd', 'e' >> Base;
        typedef Base::PresenceValidator PresenceValidator;
        typedef Base::ValueValidator ValueValidator;
        typedef NetPacketMode ModeType;


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
        // Signature of functions and possible values
        // {
        //      <PRESENCE VALIDATION/NULLPTR (HAS/OR NOT) -> { VALID/INVALID, ERR_MSG } >,
        //      <VALIDATING_FUNCTION/NULLPTR -> { VALID/INVALID, ERR_MSG }>
        // }
        // Example: <nullptr, [](value){
        //      return std::make_pair(
        //                Parse<int>(value) < 10,
        //                fmt::fmt("Value '{}' is higher than max allowed (10)"
        //              );}>
        typedef std::pair<PresenceValidator, ValueValidator> ValidatorEntry;


        ///////////////  VARIABLES ////////////////////
        static const std::string HELP_TEXT;
        static constexpr char* MODE_OPT_NAME = "mode";
        static const std::set<std::string> VALUED_OPTS, FLAG_OPTS;
        static const std::map<std::string, std::pair<PresenceValidator, ValueValidator>> VALIDATORS;
        ////////////// METHODS /////////

        ////       CONSTRUCTORS
        NetPacketCommandLine(): Base(VALUED_OPTS, FLAG_OPTS, VALIDATORS, HELP_TEXT) {}

        using Base::isOptFlag;
        using Base::getFlagVal;
        using Base::isOptSet;
        using Base::getOpt;
        using Base::start;
        using Base::setOpt;
        using Base::setMode_;

        using Base::Parse;
    };
}




#endif //NPCT_CMD_IMPL_NETPACKET_COMMANDLINE_H
