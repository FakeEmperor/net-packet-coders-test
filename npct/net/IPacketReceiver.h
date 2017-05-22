#ifndef NPCT_DATA_IPACKETRECEIVER_H
#define NPCT_DATA_IPACKETRECEIVER_H

#include "IDataReceiver.h"


namespace npct::net
{
    
    template <class Impl_, class PeerSessionClass_>
    class IPacketReceiver : public IReceiver<Impl_, PeerSessionClass_>
    {
    public:

        typedef typename PeerSessionClass_::PacketType PacketType;
        typedef IPacketReceiver<Impl_, PeerSessionClass_> Base;

        typedef npct::common::HandlerFn<const Impl_ *, PeerSessionClass_ *, const PacketType*> OnPacketReceiveFn;

        virtual void onPacketReceive(const OnPacketReceiveFn &) = 0;

    };

}

#endif // NPCT_DATA_IPACKETRECEIVER_H