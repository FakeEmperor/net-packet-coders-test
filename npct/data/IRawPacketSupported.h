#ifndef NPCT_DATA_IRAWPACKETSUPPORTED_H
#define NPCT_DATA_IRAWPACKETSUPPORTED_H

#include <memory>
#include <utility>

namespace npct::data
{

    template <class Impl_, typename RawType = char, typename RawTypeStorage = std::unique_ptr<RawType>>
    class IRawPacketSupported
    {
    public:
        typedef Impl_ Impl;

        virtual std::pair<RawTypeStorage, size_t> to_raw() const = 0;
            

        virtual ~IRawPacketSupported() = default;
            
        static Impl_ from_raw(const RawType* data, size_t size)
        {
            return Impl::from_raw(data);
        }
    };

}


#endif // !NPCT_DATA_IRAWPACKETSUPPORTED_H
