#ifndef NPCT_CODERS_IMPL_ALGO_HAMMING_HAMMINGCODER_H
#define NPCT_CODERS_IMPL_ALGO_HAMMING_HAMMINGCODER_H

#include "../../../ICoder.h"
#include "Hamming.h"
#include "HammingIOMessage.h"
#include <log/ILogger.h>

namespace npct::coders::impl::algo::hamming
{
    template<size_t PacketBits, size_t InfoBits>
    class HammingCoder : public ICoder
    {
        static_assert(PacketBits == 7 && InfoBits == 4, "Hamming code only implemented for (7,4) configuration. Sorry :(");

        using Hamming = algo::hamming::Hamming<PacketBits, InfoBits>;
    public:
        ~HammingCoder() override = default;
        HammingCoder() = default;
        HammingCoder(const HammingCoder &) = default;
        HammingCoder(HammingCoder &&) noexcept = default;

        HammingCoder& operator = (const HammingCoder &) = default;
        HammingCoder& operator = (HammingCoder &&) noexcept = default;

        static HammingCoder<PacketBits, InfoBits>::HolderType<CoderResult>
        encode(const HammingIOMessage &to_encode);
        static HammingCoder<PacketBits, InfoBits>::HolderType<CoderResult>
        decode(const HammingIOMessage &to_decode);

        const char* name() const override;
        
        HolderType<CoderResult> processInput(RefType<const CoderMessage> &source) const override;
        HolderType<CoderResult> processOutput(RefType<const CoderMessage> &encoded) const override;

        static void SetLogger(const std::shared_ptr<log::ILogger<>> &logger);
    private:
        static std::shared_ptr<log::ILogger<>> logger_;


    };


}
#include "HammingCoder.tcc"
#endif // !NPCT_CODERS_IMPL_ALGO_HAMMING_HAMMINGCODER_H 