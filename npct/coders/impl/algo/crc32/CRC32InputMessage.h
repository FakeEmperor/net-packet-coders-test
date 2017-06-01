#ifndef NPCT_CODERS_IMPL_ALGO_CRC32_CRC32INPUTMESSAGE_H
#define NPCT_CODERS_IMPL_ALGO_CRC32_CRC32INPUTMESSAGE_H
#include "../../../CoderMessage.h"
#include <string>

namespace npct::coders::impl::algo::crc32
{

    // для входа кодера сделаем поле "input"
    // Отнаследуемся от CoderMessage, чтобы внутренняя магия могла работать
    struct CRC32InputMessage : public coders::CoderMessage
    {
        // Э, нет :(
        CRC32InputMessage(const CoderMessage &base) : input(base.underlying["input"].get<std::string>()) { }
        CRC32InputMessage() = default;

        std::string input;

        nlohmann::json to_json() const override
        {
            underlying["input"] = input;
            return underlying; // <-- эта переменная хранит джсон-представление сообщения. Вообще, можно собирать джсон с нуля, но я лучше его просто модифицирую
        }
    };

}



#endif // !NPCT_CODERS_IMPL_ALGO_CRC32_CRC32INPUTMESSAGE_H