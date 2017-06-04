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
        HammingIOMessage(const CoderMessage &base) : CoderMessage(base), data(base.underlying["data"].get<std::string>()) {}

        std::string data;

    };

}

#endif // !NPCT_CODERS_IMPL_ALGO_HAMMING_HAMMINGIOMESSAGE_H


