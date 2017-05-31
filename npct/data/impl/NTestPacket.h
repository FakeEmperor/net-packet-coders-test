#ifndef NPCT_DATA_NTESTPACKET_H
#define NPCT_DATA_NTESTPACKET_H

#include <memory>
#include <json.hpp>
#include <data/IRawSerializable.h>
#include <data/IJsonSerializable.h>

namespace npct::data::impl
{
    struct NTestPacket final: IRawSerializable<NTestPacket>, IJsonSerializable<NTestPacket>
    {
    public:
        enum ControlFlags : char
        {
            CF_DATA_NOT_NOISED = 0x01, 
            CF_CONTROL_PACKET = 0x02,
            CF_NONE = 0x00,
        } ;
        
        ControlFlags flags = { CF_NONE };
        nlohmann::json underlying = { };

        NTestPacket() = default;
        NTestPacket(const NTestPacket &) = default;
        NTestPacket(NTestPacket &&) = default;
        NTestPacket(ControlFlags flags, nlohmann::json underlying) : flags(flags), underlying(std::move(underlying)) {};


        nlohmann::json to_json() const override 
        {
            return underlying;
        }

        static NTestPacket from_json(const nlohmann::json &j)
        {
            return NTestPacket((ControlFlags)j["flags"].get<char>(), j);
        }

        std::pair<std::unique_ptr<char[]>, size_t> to_raw() const override 
        {
            auto s = this->to_json().dump();
            auto v = std::make_unique<char[]>(s.size());
            std::copy(s.begin(), s.end(), v.get());
            return std::make_pair(std::move(v), s.size());
        }

        static NTestPacket from_raw(const char* data, size_t size)
        {
            return from_json(nlohmann::json::parse(data, data+size));
        }
    };
}



#endif // NPCT_DATA_NTESTPACKET_H
