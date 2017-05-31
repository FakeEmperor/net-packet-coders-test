#ifndef NPCT_CODERS_IMPL_NETCODERSRUNNER_H
#define NPCT_CODERS_IMPL_NETCODERSRUNNER_H
#include "CodersRunner.h"
#include <net/impl/JsonNetReceiver.h>

namespace npct::coders::impl
{
    class NetCodersRunner: public CodersRunner, protected net::impl::JsonNetReceiver<CoderMessage>
    {
    public:
        NetCodersRunner(asio::io_service &io_loop, uint16_t port);
        ~NetCodersRunner() override = default;


        void start() override;
    protected:
        using ReceiverBase = net::impl::JsonNetReceiver<CoderMessage>;
        using SessionObject = typename ReceiverBase::SessionClass;
        using MessageObject = CoderMessage;

        void handle_receive_message_(const ReceiverBase *, SessionObject *, const MessageObject *) const;


    };


}


#endif // !NPCT_CODERS_IMPL_NETCODERSRUNNER_H
