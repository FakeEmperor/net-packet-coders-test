#ifndef NPCT_CODERS_IMPL_ALGO_CRC32_CRC32OUTPUTMESSAGE_H
#define NPCT_CODERS_IMPL_ALGO_CRC32_CRC32OUTPUTMESSAGE_H
#include "../../../CoderMessage.h"

namespace npct::coders::impl::algo::crc32
{
    
    // ��� ������ ������ ������� ���� "checksum"
    // ������������� �� CoderMessage, ����� ���������� ����� ����� ��������
    struct CRC32OutputMessage : public coders::CoderMessage
    {
        // ��� ����, ����� �� ����� ����� ������-����������, ���� �������������� ���� ����� � ������� ����������� 
        // ��� �����:
        CRC32OutputMessage(const CoderMessage &base) 
        : checksum(base.underlying["checksum"].get<size_t>()), CoderMessage(base) { }
        CRC32OutputMessage() = default;

        size_t checksum;

        nlohmann::json to_json() const override // <-- ����� ��, ����� �� ������, ��� ������ � 17 ������ ������
        {
            underlying["checksum"] = checksum;
            underlying["message_direction"] = MT_OUTPUT;
            return this->underlying; // <-- ��� ���������� ������ �����-������������� ���������. ������, ����� �������� ����� � ����, �� � ����� ��� ������ �����������
        }

    };
    // ��� ������ ��.
}



#endif // !NPCT_CODERS_IMPL_ALGO_CRC32_CRC32OUTPUTMESSAGE_H



