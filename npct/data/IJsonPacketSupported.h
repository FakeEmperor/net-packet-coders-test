#ifndef NPCT_DATA_IJSONPACKETSUPPORTED_H
#define NPCT_DATA_IJSONPACKETSUPPORTED_H

#include <json.hpp>

namespace npct::data
{
    template <class Impl_>
    class IJsonPacketSupported
    {
    public:
        typedef Impl_ Impl;

        virtual ~IJsonPacketSupported() = default;

        virtual nlohmann::json to_json() const = 0;

        static Impl_ from_json(const nlohmann::json &json)
        {
            return Impl_::from_json(json);
        }
    };

}


#endif // !NPCT_DATA_IJSONPACKETSUPPORTED_H
