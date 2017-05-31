#ifndef NPCT_CODERS_IMPL_ALGO_CRC32_CRC32INPUTMESSAGE_H
#define NPCT_CODERS_IMPL_ALGO_CRC32_CRC32INPUTMESSAGE_H
#include "../../../CoderMessage.h"
#include <string>

namespace npct::coders::impl::algo::crc32
{

    // ��� ����� ������ ������� ���� "input"
    // ������������� �� CoderMessage, ����� ���������� ����� ����� ��������
    struct CRC32InputMessage : public coders::CoderMessage
    {
        // �, ��� :(
        CRC32InputMessage(const CoderMessage &base) : input(base.underlying["input"].get<std::string>()) { }
        CRC32InputMessage() = default;

        std::string input;

        nlohmann::json to_json() const override
        {
            underlying["input"] = input;
            return underlying; // <-- ��� ���������� ������ �����-������������� ���������. ������, ����� �������� ����� � ����, �� � ����� ��� ������ �����������
        }
    };

}



#endif // !NPCT_CODERS_IMPL_ALGO_CRC32_CRC32INPUTMESSAGE_H