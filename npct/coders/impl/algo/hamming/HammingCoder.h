#ifndef NPCT_CODERS_IMPL_ALGO_HAMMING_HAMMINGCODER_H
#define NPCT_CODERS_IMPL_ALGO_HAMMING_HAMMINGCODER_H

#include "../../../ICoder.h"

namespace npct::coders::impl::algo::hamming
{
    template<size_t PacketBits, size_t InfoBits>
    class HammingCoder : public ICoder
    {
        static_assert(PacketBits == 7 && InfoBits == 4, "Hamming code only implemented for (7,4) configuration. Sorry :(");

    public:
        ~HammingCoder() override = default;
        HammingCoder() = default;
        HammingCoder(const HammingCoder &) = default;
        HammingCoder(HammingCoder &&) noexcept = default;

        HammingCoder& operator = (const HammingCoder &) = default;
        HammingCoder& operator = (HammingCoder &&) noexcept = default;

        const char* name() const override;
        
        HolderType<CoderResult> processInput(RefType<const CoderMessage> &source) const override;
        HolderType<CoderResult> processOutput(RefType<const CoderMessage> &encoded) const override;
    };


}
#include "HammingCoder.tcc"
#endif // !NPCT_CODERS_IMPL_ALGO_HAMMING_HAMMINGCODER_H 