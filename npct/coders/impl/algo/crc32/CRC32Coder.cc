#include "CRC32Coder.h"
#include <string>

namespace npct::coders::impl::algo::crc32
{
    /**
    *
    * \see https://barrgroup.com/Embedded-Systems/How-To/CRC-Calculation-C-Code
    */
    unsigned long CRC32Coder::algo(const std::string &buf)
    {
        uint32_t crc_table[256];
        uint32_t crc;
        // precompute table
        for (uint16_t i = 0; i < 256; i++)
        {
            crc = i;
            for (uint8_t j = 0; j < 8; j++)
                crc = crc & 0x01 ? (crc >> 1) ^ 0xEDB88320UL : crc >> 1;

            crc_table[i] = crc;
        }

        crc = 0xFFFFFFFFUL;

        size_t s = buf.size() - 1;
        const char* buf_ptr = buf.data();
        while ((s--) != -1)
            crc = crc_table[(crc ^ *buf_ptr++) & 0xFF] ^ (crc >> 8);

        return crc ^ 0xFFFFFFFFUL;
    }


    CRC32Coder::HolderType<CoderResult> CRC32Coder::processInput(RefType<const CoderMessage> &source) const {
        CRC32InputMessage input = source->convert_to<CRC32InputMessage>();
        auto checksum = algo(input.input);

        auto result = std::make_unique<CRC32OutputMessage>();
        result->checksum = checksum;
        result->underlying = source->underlying;


        return std::make_unique<CoderResult>(CoderResult(CoderResultInfo(CoderResultInfo::CR_NONE, CoderError(0), true), std::move(result)));
    }

    CRC32Coder::HolderType<CoderResult> CRC32Coder::processOutput(RefType<const CoderMessage> &encoded) const {
        return std::make_unique<CoderResult>(CoderResult(CoderResultInfo(), nullptr));
    }

    const char* CRC32Coder::name() const
    {
        return "crc32";
    }
}

