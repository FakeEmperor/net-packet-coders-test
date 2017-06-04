#ifndef NPCT_COMMON_ERRORS_ERRORS_H
#define NPCT_COMMON_ERRORS_ERRORS_H

#include <stdexcept>
#include <fmt/format.h>

namespace npct::common::errors
{
    class NotImplementedError: public std::runtime_error
    {
    public:
        virtual ~NotImplementedError() = default;

        NotImplementedError(const NotImplementedError &) = default;
        NotImplementedError() 
        : std::runtime_error("Method is not yet implemented") {};
        NotImplementedError(const std::string &method) 
        : std::runtime_error(fmt::format("Method '{}' is not yet implemented", method)) { }
        NotImplementedError(const std::string &method, const std::string &message) 
        : std::runtime_error(fmt::format("Method '{}' is not yet implemented: {}", method, message)) { }
    };

    class ValueError : public std::runtime_error
    {
    public:
        virtual ~ValueError() = default;

        ValueError(const ValueError &) = default;

        ValueError()
            : std::runtime_error("Variable's value is invalid") {};
        ValueError(const std::string &variable)
            : std::runtime_error(fmt::format("Variable '{}' has invalid value", variable)) { }
        ValueError(const std::string &variable, const std::string &value_repr)
            : std::runtime_error(fmt::format("Variable '{}' has invalid value '{}'", variable, value_repr)) { }

        ValueError(const std::string &variable, const std::string &value_repr, const std::string &message)
            : std::runtime_error(fmt::format("Variable '{}' has invalid value: '{}': {}", variable, value_repr, message)) { }
        
    };

}

#endif // !NPCT_COMMON_ERRORS_ERRORS_H
