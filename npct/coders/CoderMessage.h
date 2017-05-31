#ifndef NPCT_CODERS_ICODERMESSAGE_H
#define NPCT_CODERS_ICODERMESSAGE_H


#include <data/IJsonSerializable.h>
#include <data/IRawSerializable.h>

#include <utils/utils.h>
#include <utils/compile_time.h>

#include <utility>

namespace npct::coders
{


    struct CoderMessage : public data::IJsonSerializable<CoderMessage>, public data::IRawSerializable<CoderMessage>
    {
    public:
        virtual ~CoderMessage() override = default;

        mutable nlohmann::json underlying = {  };
        std::string algorithm = { "" };
        enum MessageDirectionType : uint8_t { MT_INPUT = 0x01, MT_OUTPUT = 0x02, MT_NONE = 0xFF } message_direction = { };

        CoderMessage() = default;
        CoderMessage(const CoderMessage &) = default;
        CoderMessage& operator =(const CoderMessage &) = default;
        CoderMessage(std::string algorithm, const MessageDirectionType &message_type, nlohmann::json underlying) : underlying(std::move(underlying)), algorithm(std::move(algorithm)), message_direction(message_type) {};


        GENERATE_COMPILE_TIME_METHOD_CHECK(convert_to, false)

        template <class Impl>
        using __has_convert_to = has_method_convert_to<const CoderMessage, Impl()>;
        template <typename Impl>
        using __is_constructible_from_base = typename npct::utils::compile_time::constructible<Impl, const CoderMessage &>::from;

        // override this to correctly set-up your message after it is detected
        template <class Impl>
        typename std::enable_if<not __has_convert_to<Impl>::value and not __is_constructible_from_base<Impl>::value, Impl>::type convert_to() const
        {
            // sizeof(T) ALWAYS false for any type but it can alleviate the problem
            // @see https://stackoverflow.com/questions/14637356/static-assert-fails-compilation-even-though-template-function-is-called-nowhere
            static_assert(sizeof(Impl)==0,
                          "Fallback to default 'convert_to<Impl>()' method is prohibited. Provide your own 'template<> npct::coders::CoderMessage::convert_to<Impl>() const' "
                                  "method with your 'Impl' type or provide a constructor from 'const CoderMessage &' class.");
        }

        template <class Impl>
        typename std::enable_if<__has_convert_to<Impl>::value, Impl>::type convert_to() const
        {

            return typename Impl::convert_to(*this);
        }

        template <class Impl>
        typename std::enable_if<not __has_convert_to<Impl>::value and __is_constructible_from_base<Impl>::value, Impl>::type convert_to() const
        {
            return Impl(*this);
        }

        nlohmann::json to_json() const override {
            // BUG: Probably can lead to bugs. Maybe fix this? Like constructing real json
            return underlying;
        }


        // you can override this, but your method WON'T be called upon receiving messages and parsing them
        static CoderMessage from_json(const nlohmann::json &j)
        {
            return CoderMessage(j["algorithm"].get<std::string>(), static_cast<MessageDirectionType>(j["message_direction"].get<uint8_t>()), j);
        }

        static CoderMessage from_raw(const char *data, size_t size)
        {
            return from_json(nlohmann::json::parse(data, data + size));
        }

        std::pair<std::unique_ptr<char[]>, size_t> to_raw() const override {
            auto s = this->to_json().dump();
            auto v = std::make_unique<char[]>(s.size());
            std::copy(s.begin(), s.end(), v.get());
            return std::make_pair(std::move(v), s.size());
        }
    };
}


#endif // !NPCT_CODERS_ICODERMESSAGE_H
