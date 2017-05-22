#include "cmd.h"

namespace npct::cmd {

    void CommandProcessor::opt_check_(const std::string &name) const noexcept(false) {
        if (flag_options_.find(name) == flag_options_.cend())
            throw std::runtime_error("This flag option does not exist");
    }

    void CommandProcessor::v_opt_check_(const std::string &name) const noexcept(false) {
        if (valued_options_.find(name) == valued_options_.cend())
            throw std::runtime_error("This valued option does not exist");
    }

    std::map<std::string, std::string> CommandProcessor::currentOptions() const {
        return this->current_options_;
    }

    void CommandProcessor::setOpt(const std::string &name) noexcept(false) {
        opt_check_(name);
        current_options_[name] = "";
    }

    void CommandProcessor::setOpt(const std::string &name, const std::string &value) noexcept(false) {
        v_opt_check_(name);
        current_options_[name] = value;
    }

    bool CommandProcessor::getFlagVal(const std::string &name) const noexcept {
        opt_check_(name);
        return isOptSet(name);
    }

    bool CommandProcessor::isOptSet(const std::string &name) const noexcept {
        return current_options_.find(name) != current_options_.cend();
    }

    bool CommandProcessor::isOptFlag(const std::string &name) const noexcept(false) {
        if (flag_options_.find(name) != flag_options_.cend())
            return true;
        if (valued_options_.find(name) == valued_options_.cend())
            throw std::runtime_error("Option '-" + name + "' does not exist."); // verify existence
        return false;
    }


    std::pair<CommandProcessor::TokenType, std::string>
    CommandProcessor::ProcessToken(const std::string &token) noexcept(false) {
        constexpr size_t stepcheck_limit = 1;
        size_t step = 0;
        auto fsm_step = [&step, &stepcheck_limit, &token]() -> bool {
            if (token.size() == step)
                throw std::runtime_error("Token '" + token + "' has invalid size for this token type.");
            if (token[step] == '-') {
                if (step == stepcheck_limit)
                    throw std::runtime_error("Invalid token '" + token + "' - too much '-' chars");
                else
                    return true;
            } else
                return false;
        };
        // run finite state machine
        while (fsm_step())
            ++step;

        auto type = TokenType(step);
        size_t bg = step, ed = 0;
        // post fsm filtering
        if (type == TokenType::OPTION_VALUE) {
            // filter out \" chars
            if (token[0] == '\"') {
                bg = 1;
                size_t ps = token.find_last_of('\"');
                if (ps == token.npos)
                    throw std::runtime_error("Unclosed \" char in token " + token);
                ed = token.size() - ps;
            }
        }
        auto val = std::string(token.begin() + bg, token.end() - ed);
        return std::make_pair<TokenType, std::string>(std::move(type), std::move(val));
    }
}