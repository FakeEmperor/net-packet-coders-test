#ifndef NPCT_CODERS_IMPL_ALGO_HAMMING_HAMMINGIOMESSAGE_H
#define NPCT_CODERS_IMPL_ALGO_HAMMING_HAMMINGIOMESSAGE_H
#include "../../../CoderMessage.h"
#include <string>

namespace npct::coders::impl::algo::hamming
{
    struct HammingIOMessage : public CoderMessage
    {
    public:
        ~HammingIOMessage() override = default;

        HammingIOMessage() = default;
        HammingIOMessage(const HammingIOMessage &) = default;
        HammingIOMessage(HammingIOMessage &&) noexcept = default;

        HammingIOMessage(const CoderMessage &base) : CoderMessage(base), input(base.underlying["input"].get<std::string>()) {}

        nlohmann::json to_json() const override {
            underlying["input"] = input;
            // stats
            auto j = nlohmann::json();
            j["decode_errors"] = decode_errors;

            underlying["hamming"] = j;
            return underlying;
        }

        std::string input;
        size_t decode_errors = { 0 };
    };

}

#endif // !NPCT_CODERS_IMPL_ALGO_HAMMING_HAMMINGIOMESSAGE_H


