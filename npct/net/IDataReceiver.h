#ifndef NPCT_DATA_IRECEIVER_H
#define NPCT_DATA_IRECEIVER_H

#include <common/callbacks.h>
#include "IPeerSession.h"

namespace npct::net
{

    template <typename Impl_, class PeerSessionClass_, class BufferClass_>
    class IDataReceiver
    {
    public:
        using PeerSessionClass = PeerSessionClass_;
        using BufferClass = typename PeerSessionClass::BufferClass;
        typedef Impl_ Impl;
        static_assert(std::is_base_of<IPeerSession<PeerSessionClass_, typename PeerSessionClass::SessionSettingsClass, BufferClass >, PeerSessionClass>::value,
                      "PeerSessionClass_ must be a subclass of IPeerSession with MessageType_ = PeerSessionClass_::BufferType");
        

        typedef IDataReceiver<Impl_, PeerSessionClass_, BufferClass_> Base;


        typedef common::HandlerFn<const Impl_ *, PeerSessionClass *, const BufferClass &> OnPartialReceiveFn;
        typedef common::HandlerFn<const Impl_ *, PeerSessionClass *> OnPeerConnectFn;

        virtual ~IDataReceiver() = default;

        virtual void start() = 0;

        virtual void closeSession(const PeerSessionClass *) = 0;

        virtual void onPartialReceive(const OnPartialReceiveFn &) = 0 ;
        virtual void onPeerConnect(const OnPeerConnectFn &) = 0;

        template <class ...Args>
        Impl_ Build(Args ...a)
        {
            return Impl_::Ptr(new Impl_(a...));
        }
    };

}



#endif // NPCT_DATA_IRECEIVER_H
