#ifndef NPCT_DATA_NTESTPACKET_H
#define NPCT_DATA_NTESTPACKET_H

#include <memory>
#include <json.hpp>
#include <data/IRawPacketSupported.h>
#include <data/IJsonPacketSupported.h>

namespace npct::data::impl
{
    class NTestPacket final: IRawPacketSupported<NTestPacket>, IJsonPacketSupported<NTestPacket>
    {
    public:
        enum ControlFlags : char
        {
            CF_DATA_NOT_NOISED = 0x01, 
            CF_CONTROL_PACKET = 0x02,
        } ;
        
        ControlFlags flags;
        std::vector<char> data;

        NTestPacket() = default;
        NTestPacket(ControlFlags flags, std::vector<char> data) : flags(flags), data(std::move(data)) {};


        nlohmann::json to_json() const override 
        {
            nlohmann::json j;
            j["flags"] = (char)flags;
            j["data"] = std::string(data.begin(), data.end());
            return j;
        }

        static NTestPacket from_json(const nlohmann::json &j)
        {
            auto str = j["data"].get<std::string>();
            return NTestPacket((ControlFlags)j["flags"].get<char>(), std::vector<char>(str.begin(), str.end()));
        }

        std::pair<std::unique_ptr<char>, size_t> to_raw() const override 
        {
            auto s = this->to_json().dump();
            auto v = std::make_unique<char>(s.size());
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
