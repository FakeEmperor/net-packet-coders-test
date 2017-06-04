#include "HammingCoder.h"
#include "HammingIOMessage.h"
#include <common/errors/errors.h>

namespace npct::coders::impl::algo::hamming
{
    template<size_t PacketBits, size_t InfoBits>
    const char* HammingCoder<PacketBits, InfoBits>::name() const
    {
        return "hamming";
    }

    template<size_t PacketBits, size_t InfoBits>
    typename HammingCoder<PacketBits, InfoBits>::HolderType<CoderResult> 
    HammingCoder<PacketBits, InfoBits>::processInput(RefType<const CoderMessage> &source) const {
        throw common::errors::NotImplementedError();
    }

    template<size_t PacketBits, size_t InfoBits>
    typename HammingCoder<PacketBits, InfoBits>::HolderType<CoderResult>  
    HammingCoder<PacketBits, InfoBits>::processOutput(RefType<const CoderMessage> &encoded) const {
        throw common::errors::NotImplementedError();
    }
}
