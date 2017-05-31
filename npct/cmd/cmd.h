#pragma once
#ifndef NPCT_CMD_CMD_H
#define NPCT_CMD_CMD_H
#ifdef _MSC_VER
#include <iso646.h>
#endif
#include <string>
#include <map>
#include <functional>
#include <set>
#include <locale>
#include <utility>
#include <vector>
#include <memory>
#include <iostream>
#include <cstring>
#include <sstream>
#include <list>

#include <utils/utils.h>
#include <utils/static_if.h>
#include <fmt/format.h>


namespace npct::cmd {

    class CommandProcessor
    {

    public:
        ////////////////// SUPPORT TYPES ////////////

         enum TokenType
         {
            OPTION_VALUE = 0,
            OPTION_NAME = 1,
         };

        ///////////////// METHODS ////////////////
        //////      CONSTRUCTORS

        CommandProcessor(std::set<std::string> valued_options, std::set<std::string> flag_options) :
                valued_options_(std::move(valued_options)),  flag_options_(std::move(flag_options))
        {
            // validate
            for (auto &v : valued_options_)
                if (flag_options_.find(v) != flag_options_.cend())
                    throw std::runtime_error("Duplicated option");
            for (auto &v : flag_options_)
                if (valued_options_.find(v) != valued_options_.cend())
                    throw std::runtime_error("Duplicated option");
        }

        CommandProcessor(const CommandProcessor&) = default;
        CommandProcessor(CommandProcessor&&) = default;

        CommandProcessor& operator =(CommandProcessor&&) = delete;
        CommandProcessor& operator =(const CommandProcessor&) = delete;

        virtual ~CommandProcessor() = default;

        /////       ATTRIBUTE GETTERS

		virtual std::map<std::string, std::string> currentOptions() const;


		virtual void setOpt(const std::string &name) noexcept(false);
		virtual void setOpt(const std::string &name, const std::string& value) noexcept(false);

		template <typename DesiredType>
		DesiredType getOpt(const std::string &name) const noexcept(false)
		{
			v_opt_check_(name);
			auto val = current_options_.at(name);
			return Parse<DesiredType>(val);
		}


		bool getFlagVal(const std::string &name) const noexcept;
		bool isOptSet(const std::string &name) const noexcept;
		bool isOptFlag(const std::string &name) const noexcept(false);


        //////      PARSERS

        static std::pair<TokenType, std::string> ProcessToken(const std::string& token) noexcept(false);

        template <typename DesiredType>
        static DesiredType Parse(const std::string &val) noexcept(false)
        {
            std::stringstream ss(val);
            DesiredType var;
            ss >> var;
            return var;
        }


         ///////// TEMPLATE SPECIFICATION FOR PARSING TYPES


        template <template <typename> typename ContainerType, typename IntOrFloatType, typename = typename std::enable_if<std::is_integral<IntOrFloatType>::value || std::is_floating_point<IntOrFloatType>::value>::type>
        static ContainerType<IntOrFloatType> Parse(const std::string& val) noexcept(false)
        {
            auto vs = utils::split(val, ",");
            return ContainerType<IntOrFloatType>(vs.begin(), vs.end());
        }
    protected:
        std::set<std::string> valued_options_, flag_options_;

        std::map<std::string, std::string> current_options_;

        void opt_check_(const std::string &name) const noexcept(false);
        void v_opt_check_(const std::string &name) const noexcept(false);
    };









    // Adds ability to validate options, use modes and call methods from this class after parsing arguments
    template <typename ModeType = std::string, class ModeExtractFn = void, class ModeName = void>
    class CommandLine: public CommandProcessor {
    public:
        template <typename ArgType>
        using ValidatorFn = std::function<std::pair<bool, std::string>(const CommandLine &, const std::string &, const ArgType &)>;
        typedef std::function<void(const CommandLine &)> ModeFn;

        using PresenceValidator = ValidatorFn<bool> ;
        using ValueValidator = ValidatorFn<std::string>;


        CommandLine() = default;
        CommandLine(const std::set<std::string> &valued_options, const std::set<std::string> &flag_options,
                    std::map<std::string, std::pair<PresenceValidator, ValueValidator>> validators,
                    const std::string &help_text,
                    std::map<ModeType, ModeFn> mode_fns = { {} }
        ) : CommandProcessor(valued_options, flag_options),
            validators_(std::move(validators)), help_text_(help_text),
            mode_fns_(std::move(mode_fns)) { };


        virtual ModeType getMode() {
            return mode_;
        }

        virtual void runWithMode(const ModeType &mode) {
            auto fn = mode_fns_.find(mode);
            if (fn == mode_fns_.cend())
                throw std::runtime_error(fmt::format("Unknown mode '{}' to run", mode_));
            fn->second(*this);
        }

        virtual void runFromMode() {
            runWithMode(mode_);
        }

        void start(const std::vector<const char*> &args) {
            // print_help
            if (printHelp_(args)) return;
            // anyway, proceed as normal
            parseArgs_(args);
            setMode_();
            runFromMode();
        }

    protected:
        const std::map<std::string, std::pair<PresenceValidator, ValueValidator>> validators_ = { {} };
        const std::string help_text_;
        std::map<ModeType, ModeFn> mode_fns_;

        ModeType mode_;



        virtual void parseArgs_(const std::vector<const char*> &args) noexcept(false) {
            struct {
                std::set<TokenType> expected_types;
                bool expect_any = {false};
                bool expect_next = {false};
                std::string previous_token;
            } state;
            state.expected_types = {OPTION_NAME};

            std::string last_option = "";
            for (int i = 0, s = args.size(); i <s; ++i) {
                auto processed = this->ProcessToken(args[i]);
                if (!state.expect_any && state.expected_types.find(processed.first) == state.expected_types.cend())
                    throw std::runtime_error("Unexpected token: '" + std::string(args[i]) + "'");
                // state transition and processing
                switch (processed.first) {
                    case OPTION_NAME: {
                        bool is_flag = this->isOptFlag(processed.second);
                        if (is_flag)
                            this->setOpt(processed.second);
                        else {
                            state.expected_types = {OPTION_VALUE};
                            last_option = processed.second;
                        }
                        state.expect_next = !is_flag;
                        state.expect_any = is_flag;
                    }

                        break;
                    case OPTION_VALUE:
                        state.expect_any = false;
                        state.expect_next = false;
                        state.expected_types = {OPTION_NAME};
                        this->setOpt(last_option, processed.second);
                        break;
                }
            }
            // post-process check
            if (state.expect_next)
                throw std::runtime_error("Unexpected end of command-line arguments");

            // validate all options
            for (auto &name_validator : this->validators_) {
                auto opt = this->current_options_.find(name_validator.first);
                bool has_opt = opt != this->current_options_.cend();
                if (name_validator.second.first != nullptr) {
                    auto valid = name_validator.second.first(*this, name_validator.first, has_opt);
                    if (!valid.first)
                        throw std::runtime_error("Validation failed [Presence check]: '" + valid.second + "'");
                }
                if (has_opt && name_validator.second.second != nullptr) {
                    auto valid = name_validator.second.second(*this, name_validator.first, opt->second);
                    if (!valid.first)
                        throw std::runtime_error("Validation failed [Value check]: '" + valid.second + "'");
                }
            }
        }

        virtual bool printHelp_(const std::vector<const char*> &args) {
            // check if help needed
            if (args.size() == 0) {
                std::cout << help_text_;
                return true;
            }
            for (auto &a : args) {
                if (std::strcmp(a, "-help") == 0 || std::strcmp(a, "-h") == 0) {
                    std::cout << help_text_;
                    return true;
                }
            }
            return false;
        }

    public:
        template <typename Fn = ModeExtractFn, class Nm = ModeName>
        typename std::enable_if<std::is_void<Fn>::value && std::is_void<Nm>::value, void>::type
        setMode_() {

            // do nothing - user haven't set neither ModeExtractFn nor
            #pragma message __FILE__ ": ["  " CommandLine uses no mode, are you sure this is intended?" " ]"
        }



        template <typename Fn = ModeExtractFn, class Nm = ModeName>
        typename std::enable_if<std::is_void<Fn>::value and !std::is_void<Nm>::value, void>::type
        setMode_() {
            mode_ = getOpt<ModeType>(ModeName::value);
        }

        template <typename Fn = ModeExtractFn, class Nm = ModeName>
        typename std::enable_if<!std::is_void<Fn>::value and std::is_void<Nm>::value, void>::type
        setMode_() {
            mode_ = ModeExtractFn(*this);
        }

        template <typename Fn = ModeExtractFn, class Nm = ModeName>
        typename std::enable_if<!std::is_void<Fn>::value and !std::is_void<Nm>::value, void>::type
        setMode_() {
            // If you're here from compile errors - my condolences :<
            // Try to define ModeExtractFn or ModeParamName to nullptr if you don't want modes support
            static_assert(std::is_void<Fn>::value or std::is_void<Nm>::value, "ModeParamName and ModeExtractFn are mutually exclusive");
        }



    };





}
////////////////  IMPLEMENTATION
#include "cmd.tcc"
#endif //NPCT_CMD_CMD_H