#ifndef NPCT_CODERS_IMPL_CRC32CODER_H
#define NPCT_CODERS_IMPL_CRC32CODER_H

#include <coders/ICoder.h>
#include <string>



namespace npct::coders::impl::algo
{
    struct CRC32InputMessage : public CoderMessage
    {
    public:
        explicit CRC32InputMessage(const CoderMessage& base);;

        // CRC32 input string
        std::string input;
    };

    struct CRC32OutputMessage : public CoderMessage
    {
    public:
        explicit CRC32OutputMessage(const CoderMessage& base);
        CRC32OutputMessage() = default;

        nlohmann::json to_json() const override;
        // CRC32 input string
        size_t checksum;
    };

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



#endif // !NPCT_CODERS_IMPL_CRC32CODER_H
