#ifndef NPCT_CODERS_CODERERROR_H
#define NPCT_CODERS_CODERERROR_H

#include <string>
#include <data/IJsonSerializable.h>
#include "fmt/format.h"

namespace npct::coders
{
    /**
     * \brief A JSON-serializable data struct to signal any errors occured during Coder's runtime
     *        You can extend it to provide more details on your errors.
     *
     */
    class CoderError : public data::IJsonSerializable<CoderError>, public std::runtime_error
    {
    public:
        static const CoderError NOT_AN_ERROR;

        CoderError() : CoderError(0) {};
        CoderError(const CoderError &) = default;
        CoderError(CoderError &&) noexcept = default;
        CoderError& operator =(const CoderError &) = default;

        explicit CoderError(size_t error_code) : error_code(error_code), std::runtime_error(fmt::format("[CoderError] (error code: {}) CoderError! Yay! ", error_code, msg)) {};
        CoderError(size_t error_code, const std::string &msg) : error_code(error_code), msg(msg), std::runtime_error(fmt::format("[CoderError] (error code: {}) : {}", error_code, msg)) {};
        CoderError(size_t error_code, const char *msg) : error_code(error_code), msg(msg), std::runtime_error(fmt::format("[CoderError] (error code: {}) : {}", error_code, msg)) {};

        size_t error_code = { 0 };
        std::string msg;

        ~CoderError() override = default;

        CoderError & operator = (CoderError &&) noexcept = default;

        operator bool() const { return error_code != 0; }

        nlohmann::json to_json() const override {
            nlohmann::json j;
            j["error_code"] = error_code;
            j["msg"] = msg;
            return j;
        };

    };


}


#endif // !NPCT_CODERS_CODERERROR_H
