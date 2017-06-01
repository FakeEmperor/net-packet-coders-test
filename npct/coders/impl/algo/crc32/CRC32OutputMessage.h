#ifndef NPCT_CODERS_IMPL_ALGO_CRC32_CRC32OUTPUTMESSAGE_H
#define NPCT_CODERS_IMPL_ALGO_CRC32_CRC32OUTPUTMESSAGE_H
#include "../../../CoderMessage.h"

namespace npct::coders::impl::algo::crc32
{
    
    // для выхода кодера сделаем поле "checksum"
    // Отнаследуемся от CoderMessage, чтобы внутренняя магия могла работать
    struct CRC32OutputMessage : public coders::CoderMessage
    {
        // Для того, чтобы мы могли юзать классы-наследники, надо переопределить ОДИН метод и сделать конструктор 
        // Вот такой:
        CRC32OutputMessage(const CoderMessage &base) 
        : checksum(base.underlying["checksum"].get<size_t>()), CoderMessage(base) { }
        CRC32OutputMessage() = default;

        size_t checksum;

        nlohmann::json to_json() const override // <-- опять же, забей на ошибки, это именно в 17 студии траблы
        {
            underlying["checksum"] = checksum;
            underlying["message_direction"] = MT_OUTPUT;
            return this->underlying; // <-- эта переменная хранит джсон-представление сообщения. Вообще, можно собирать джсон с нуля, но я лучше его просто модифицирую
        }

    };
    // Вот теперь всё.
}



#endif // !NPCT_CODERS_IMPL_ALGO_CRC32_CRC32OUTPUTMESSAGE_H



