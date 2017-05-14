#pragma once
#ifndef NPCT_CMD_CMD_H
#define NPCT_CMD_CMD_H

#include <string>
#include <map>
#include <functional>
#include <set>
#include <locale>
#include <vector>
#include <utils/utils.h>
#include <memory>
#include <iostream>
#include <cstring>
#include <fmt/format.h>

namespace npct::cmd {

	class CommandProcessor
	{
	protected:
		std::set<std::string> valued_options_, flag_options_;

		std::map<std::string, std::string> current_options_;

		void opt_check_(const std::string &name) const noexcept(false);
		void v_opt_check_(const std::string &name) const noexcept(false);



	public:
		enum TokenType
		{
			OPTION_VALUE = 0,
			OPTION_NAME = 1,
		};

		template <typename DesiredType>
		static DesiredType Parse(const std::string &val) noexcept(false)
		{
			throw std::runtime_error("Unknown type to parse");
		}

		CommandProcessor(const std::set<std::string> &valued_options, const std::set<std::string> &flag_options);
        CommandProcessor(const CommandProcessor&) = default;
        CommandProcessor(CommandProcessor&&) noexcept = default;


		CommandProcessor& operator =(CommandProcessor&&) = delete;
		CommandProcessor& operator =(const CommandProcessor&) = delete;

		virtual ~CommandProcessor() = default;

		std::map<std::string, std::string> currentOptions() const;


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

		static std::pair<TokenType, std::string> ProcessToken(const std::string& token) noexcept(false);

        ///////// TEMPLATE SPECIFICATION FOR PARSING TYPES
        // well, no if-constexpr and return/auto in MSVC, touche
        template <>
        inline std::string CommandProcessor::Parse<std::basic_string<char>>(const std::string& val)
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

        template <typename IntOrFloat, typename = std::enable_if<std::is_integral<IntOrFloat>::value || std::is_floating_point<IntOrFloat>::value>::type>
        inline std::vector<IntOrFloat> CommandProcessor::Parse<std::vector<IntOrFloat>>(const std::string& val) noexcept(false)
        {
            auto vs = utils::split(val, ",");
            std::vector<IntOrFloat> ret;
            for (auto& v : vs)
                ret.push_back(Parse<IntOrFloat>(v));
            return ret;
        }




    };


    // Adds ability to validate options, use modes and call methods from this class after parsing arguments
    template <typename ModeType = std::string>
    class CommandLine: public CommandProcessor {
    public:
        template <typename ArgType>
        using ValidatorFn = std::function<std::pair<bool, std::string>(const CommandLine &, const std::string &, const ArgType &)>;
        typedef std::function<void(const CommandLine &)> ModeFn;

        using PresenceValidator = ValidatorFn<bool> ;
        using ValueValidator = ValidatorFn<std::string>;

    protected:
        const std::map<std::string, std::pair<PresenceValidator, ValueValidator>> validators_ = { {} };
        const std::string mode_param_name_ = { "" }, help_text_;
        const std::function<std::string(const CommandLine &)> mode_extract_fn_ = { nullptr };
        const std::map<std::string, ModeFn> mode_fns_ = { {} };

        ModeType mode_;

        CommandLine(const std::set<std::string> &valued_options, const std::set<std::string> &flag_options,
                    const std::map<std::string, std::pair<PresenceValidator, ValueValidator>> &validators,
                    const std::string &help_text,
                    const std::string &mode_param_name,
                    const std::function<ModeType(const CommandLine &)> &mode_extract_fn,
                    const std::map<ModeType, ModeFn> &mode_fns
        ): CommandProcessor(valued_options, flag_options),
           validators_(validators), help_text_(help_text), mode_param_name_(mode_param_name),
           mode_extract_fn_(mode_extract_fn), mode_fns_(mode_fns) { };

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

        virtual void printHelp_(const std::vector<const char*> &args) {
            // check if help needed
            if (args.size() == 0) {
                std::cout << help_text_;
                return;
            }
            for (auto &a : args) {
                if (std::strcmp(a, "-help") == 0 || std::strcmp(a, "-h") == 0) {
                    std::cout << help_text_;
                    return;
                }
            }
        }

        virtual void setMode_() {
            mode_ = mode_extract_fn_ ? mode_extract_fn_(*this) : getOpt<ModeType>(mode_param_name_);
        }


    public:
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
            printHelp_(args);
            // anyway, proceed as normal
            parseArgs_(args);
            setMode_();
            runFromMode();
        }

        static std::unique_ptr<CommandLine> Build(const std::set<std::string> &valued_options, const std::set<std::string> &flag_options,
                                 const std::map<std::string, std::pair<PresenceValidator, ValueValidator>> &validators,
                                 const std::string &help_text,
                                 const std::string &mode_param_name = "",
                                 const std::function<std::string(const CommandLine &)> &mode_extract_fn = nullptr,
                                 const std::map<std::string, ModeFn> &mode_fns = {}) {
            if (mode_param_name != "" && mode_extract_fn != nullptr)
                throw std::runtime_error("mode_param_name and mode_extract are mutually exclusive");
            return std::make_unique(valued_options, flag_options, validators, help_text, mode_param_name, mode_extract_fn, mode_fns);
        }
    };


}

#endif