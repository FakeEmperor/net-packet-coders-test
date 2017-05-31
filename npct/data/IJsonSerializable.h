#ifndef NPCT_DATA_IJSONSERIALIZABLE_H
#define NPCT_DATA_IJSONSERIALIZABLE_H

#include <json.hpp>
#include <utils/compile_time.h>

namespace npct::data
{
    template <class Impl_, typename FromJsonParser = void>
    class IJsonSerializable
    {
    public:
        typedef Impl_ Impl;

        virtual ~IJsonSerializable() = default;

        virtual nlohmann::json to_json() const = 0;

        // statically enable or disable from_json method
        GENERATE_COMPILE_TIME_METHOD_CHECK(from_json, true)
        GENERATE_COMPILE_TIME_FUNC_SIG_CHECK(from_json, Impl_(*)(const nlohmann::json &))

        template <typename = typename std::enable_if
        <  std::is_same<FromJsonParser, Impl_(*)(const nlohmann::json &)>::value
           or
           has_member_func_from_json<Impl_>::value
           or
           has_method_from_json<Impl_, Impl_(const nlohmann::json &)>::value>>
        static Impl_ from_json(const nlohmann::json &json)
        {
            return FromJsonParser(json);
        }
    };

}


#endif // NPCT_DATA_IJSONSERIALIZABLE_H
