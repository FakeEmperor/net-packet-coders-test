#ifndef NPCT_DATA_IRECEIVER_H
#define NPCT_DATA_IRECEIVER_H

#include <common/handlers.h>

namespace npct::net
{

    template <typename Impl_, class PeerSessionClass_>
    class IReceiver
    {
    public:
        typedef PeerSessionClass_ PeerSessionClass;
        typedef typename PeerSessionClass_::BufferType BufferType;
        typedef Impl_ Impl;
        

        typedef IReceiver<Impl_, PeerSessionClass_> Base;


        typedef common::HandlerFn<const Impl_ *, PeerSessionClass_ *, const typename PeerSessionClass_::BufferType &> OnPartialReceiveFn;
        typedef common::HandlerFn<const Impl_ *, PeerSessionClass_ *> OnPeerConnectFn;

        virtual ~IReceiver() = default;

        virtual void start() = 0;

        virtual void closeSession(const PeerSessionClass_ *) = 0;

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
