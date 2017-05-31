#ifndef NPCT_DATA_IPACKETRECEIVER_H
#define NPCT_DATA_IPACKETRECEIVER_H

#include "IDataReceiver.h"
#include "IPeerSession.h"

namespace npct::net
{
    
    template <template <typename> class Impl_, class PacketClass_, class PeerSessionClass_>
    class IPacketReceiver : public IDataReceiver<Impl_<PacketClass_>, PeerSessionClass_, PacketClass_>
    {
    public:
        using PacketClass = PacketClass_;
        using PeerSessionClass = PeerSessionClass_;
        using Base = IPacketReceiver<Impl_, PacketClass_, PeerSessionClass_>;
        
        static_assert(std::is_base_of<IPeerSession<PeerSessionClass_, typename PeerSessionClass::SessionSettingsClass, PacketClass>, PeerSessionClass>::value,
                      "PeerSessionClass_ must be a subclass of IPeerSession with MessageType_ = PeerSessionClass_::PacketType");

        typedef npct::common::HandlerFn<const Impl_<PacketClass_> *, PeerSessionClass *, const PacketClass*> OnPacketReceiveFn;

        virtual void onPacketReceive(const OnPacketReceiveFn &) = 0;

    };

}

#endif // NPCT_DATA_IPACKETRECEIVER_H