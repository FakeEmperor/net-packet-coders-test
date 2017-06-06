#include "HammingCoder.h"
#include "HammingIOMessage.h"
#include <common/errors/errors.h>
#include <utils/base64.h>
#include <utils/hex.h>
#include <utils/binary.h>

namespace npct::coders::impl::algo::hamming
{
    template<size_t PacketBits, size_t InfoBits>
    std::shared_ptr<log::ILogger<>> HammingCoder<PacketBits, InfoBits>::logger_ = nullptr;


    template<size_t PacketBits, size_t InfoBits>
    const char* HammingCoder<PacketBits, InfoBits>::name() const
    {
        return "hamming";
    }

    template<size_t PacketBits, size_t InfoBits>
    HammingCoder<PacketBits, InfoBits>::HolderType<CoderResult>
    HammingCoder<PacketBits, InfoBits>::processInput(RefType<const CoderMessage> &source) const {
        HammingIOMessage message = source->convert_to<HammingIOMessage>();
        return this->encode(message);
    }

    template<size_t PacketBits, size_t InfoBits>
    HammingCoder<PacketBits, InfoBits>::HolderType<CoderResult>
    HammingCoder<PacketBits, InfoBits>::processOutput(RefType<const CoderMessage> &encoded) const {
        HammingIOMessage message = encoded->convert_to<HammingIOMessage>();
        return this->decode(message);
    }

    template<size_t PacketBits, size_t InfoBits>
    HammingCoder<PacketBits, InfoBits>::HolderType<CoderResult>
    HammingCoder<PacketBits, InfoBits>::encode(const HammingIOMessage &to_encode) {
        auto cri = CoderResultInfo(CoderResultInfo::StatusFlags::CR_ENCODED, CoderError::NOT_AN_ERROR, true);

        auto msg = std::move(std::make_unique<HammingIOMessage>(to_encode));
        if(logger_) logger_->log(log::LogLevel::LL_DEBUG, "[ascii] Message to encode: %s #%s\r\n",
                                 to_encode.input.c_str(), utils::to_bin(to_encode.input, " ").c_str());

        Hamming h(msg->input.c_str(), msg->input.size());
        auto encoded = std::string(h.encode().get(), h.encoded_size);
        msg->input = base64_encode(encoded.c_str(), h.encoded_size);

        if(logger_) logger_->log(log::LogLevel::LL_DEBUG, "[bin] Encoded: %s %s\r\n",
                                 encoded, utils::to_bin(encoded, " ").c_str());

        return std::make_unique<CoderResult>(CoderResultInfo(cri), std::move(msg));
    }

    template<size_t PacketBits, size_t InfoBits>
    HammingCoder<PacketBits, InfoBits>::HolderType<CoderResult>
    HammingCoder<PacketBits, InfoBits>::decode(const HammingIOMessage &to_decode) {
        auto cri = CoderResultInfo(CoderResultInfo::StatusFlags::CR_DECODED, CoderError::NOT_AN_ERROR, true);
        auto hm = std::make_unique<HammingIOMessage>();

        auto msg = std::move(std::make_unique<HammingIOMessage>(to_decode));

        if(logger_) logger_->log(log::LogLevel::LL_DEBUG, "[hex] Message to decode: %s\r\n",
                                 to_decode.input.c_str());

        msg->input = base64_decode(to_decode.input);

        if(logger_) logger_->log(log::LogLevel::LL_DEBUG, "[bin] Message to decode: %s\r\n",
                                 utils::to_bin(msg->input, " ").c_str());

        Hamming h(msg->input.c_str(), msg->input.size());
        auto decoded = std::string(h.decode().get(), h.decoded_size);
        msg->decode_errors = h.last_single_errors;
        msg->input = decoded;
        if(logger_) logger_->log(log::LogLevel::LL_DEBUG, "[ascii] Decoded message: %s\r\n",
                                 decoded.c_str());

        return std::make_unique<CoderResult>(CoderResultInfo(cri), std::move(msg));
    }

    template<size_t PacketBits, size_t InfoBits>
    void HammingCoder<PacketBits, InfoBits>::SetLogger(const std::shared_ptr<log::ILogger<>> &logger) {
        logger_ = logger;
    }
}
