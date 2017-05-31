#ifndef NPCT_CODERS_IMPL_ALGO_CRC32_CRC32CODER_H
#define NPCT_CODERS_IMPL_ALGO_CRC32_CRC32CODER_H

#include "CRC32InputMessage.h"
#include "CRC32OutputMessage.h"
#include <coders/ICoder.h>
#include <string>



namespace npct::coders::impl::algo::crc32
{
    class CRC32Coder : public ICoder
    {
    public:
        CRC32Coder() = default;
        virtual ~CRC32Coder() = default;


        static unsigned long algo(const std::string &buf);

        HolderType<CoderResult> processInput(RefType<const CoderMessage> &source) const override;
        HolderType<CoderResult> processOutput(RefType<const CoderMessage> &encoded) const override;
        const char* name() const override;
    };
}



#endif // !NPCT_CODERS_IMPL_ALGO_CRC32_CRC32CODER_H
