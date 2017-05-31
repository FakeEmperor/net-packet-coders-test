
#include <asio.hpp>
#include "JsonNetReceiver.h"
#include <net/utils/utils.h>
#include <fmt/format.h>

namespace npct::net::impl
{
    using namespace std::placeholders;
    using asio::ip::tcp;


    ///////////////////  JSONNETRECEIVER

    
    template<class JsonMessageType>
    void JsonNetReceiver<JsonMessageType>::start()
    {
        start_accept_();
    }

    template<class JsonMessageType>
    void JsonNetReceiver<JsonMessageType>::onPartialReceive(const OnPartialReceiveFn &handler)
    {
        mh_helper_.addCallback(EJsonNetReceiverEventTypes::PARTIAL_RECEIVED, handler);
    }

    template<class JsonMessageType>
    void JsonNetReceiver<JsonMessageType>::onPeerConnect(const OnPeerConnectFn &handler)
    {
        mh_helper_.addCallback(EJsonNetReceiverEventTypes::PEER_CONEECTED, handler);
    }

    template<class JsonMessageType>
    void JsonNetReceiver<JsonMessageType>::onPacketReceive(const OnPacketReceiveFn &handler)
    {
        mh_helper_.addCallback(EJsonNetReceiverEventTypes::PACKET_RECEIVED, handler);
    }

    template<class JsonMessageType>
    void JsonNetReceiver<JsonMessageType>::emitPartialReceive(SessionClass *sess, const typename SessionClass::BufferClass &d) const
    {
        mh_helper_.template triggerCallbacks<OnPartialReceiveFn>(EJsonNetReceiverEventTypes::PARTIAL_RECEIVED, this, sess, d);
    }

    template<class JsonMessageType>
    void JsonNetReceiver<JsonMessageType>::emitPacketReceive(SessionClass *sess, const typename SessionClass::PacketClass *packet) const
    {
        mh_helper_.template triggerCallbacks<OnPacketReceiveFn>(EJsonNetReceiverEventTypes::PACKET_RECEIVED, this, sess, packet);
    }

    template<class JsonMessageType>
    void JsonNetReceiver<JsonMessageType>::emitPeerConnect(SessionClass *sess) const
    {
        mh_helper_.template triggerCallbacks<OnPeerConnectFn>(EJsonNetReceiverEventTypes::PEER_CONEECTED, this, sess);
    }

    template<class JsonMessageType>
    void JsonNetReceiver<JsonMessageType>::closeSession(const SessionClass *sess)
    {
        auto sp = std::find_if(sessions_.begin(), sessions_.end(), [&](const std::unique_ptr<SessionClass> & current) { return current.get() == sess;  });
        if (sp != sessions_.end())
            sessions_.erase(sp);
    }




    template<class JsonMessageType>
    void JsonNetReceiver<JsonMessageType>::start_accept_()
    {
        sessions_.push_back(std::make_unique<SessionClass>(io_service_, this));
        acceptor_.async_accept(sessions_.back()->socket(),
            std::bind(&JsonNetReceiver::handle_accept_, this, sessions_.back().get(), _1));
    }

    template<class JsonMessageType>
    void JsonNetReceiver<JsonMessageType>::handle_accept_(SessionClass *new_session, const asio::error_code &error) {
        emitPeerConnect(new_session);
        auto e = new_session->socket().remote_endpoint();
        if (!error)
        {

            new_session->start();
        }
        else
            utils::print_error(error, e);

        start_accept_();
    }

    template<class JsonMessageType>
    JsonNetReceiver<JsonMessageType>::JsonNetReceiver(asio::io_service &io_loop, unsigned short port, bool debug_) :
            acceptor_(io_loop, tcp::endpoint(tcp::v4(), port)), io_service_(io_loop) {}

}
