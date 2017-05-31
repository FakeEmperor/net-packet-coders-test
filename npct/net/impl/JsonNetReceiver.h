#ifndef NPCT_NET_IMPL_NETRECEIVER_H
#define NPCT_NET_IMPL_NETRECEIVER_H

#include <json.hpp>

#include <data/impl/NTestPacket.h>
#include <data/IJsonSerializable.h>
#include <net/IPacketReceiver.h>
#include <net/IDataReceiver.h>
#include <common/helpers/handler_helpers.h>
#include "JsonNetSession.h"

namespace npct::net::impl
{

    template <class JsonMessageType>
    class JsonNetReceiver : IPacketReceiver<JsonNetReceiver, JsonMessageType, JsonNetSession<JsonMessageType>>
    {
        static_assert(std::is_base_of<npct::data::IJsonSerializable<JsonMessageType>, JsonMessageType>::value, "JsonMessageType must be a subclass of IJSonSerializable");
    public:
        using SessionClass = JsonNetSession<JsonMessageType>;
        using Base = IPacketReceiver<npct::net::impl::JsonNetReceiver, JsonMessageType, JsonNetSession<JsonMessageType>>;

        using OnPartialReceiveFn = typename Base::OnPartialReceiveFn;
        using OnPeerConnectFn    = typename Base::OnPeerConnectFn;
        using OnPacketReceiveFn  = typename Base::OnPacketReceiveFn;

        JsonNetReceiver(asio::io_service &io_loop, unsigned short port, bool debug_ = false);

        void start() override;

        void onPartialReceive(const OnPartialReceiveFn &handler) override;
        void onPeerConnect(const OnPeerConnectFn &handler) override;
        void onPacketReceive(const OnPacketReceiveFn &handler) override;

        void emitPartialReceive(SessionClass *, const typename SessionClass::BufferClass &) const;
        void emitPacketReceive(SessionClass *, const  typename SessionClass::PacketClass *) const;
        void emitPeerConnect(SessionClass *) const;

        void closeSession(const SessionClass *) override;
    private:
        std::vector<std::unique_ptr<SessionClass >> sessions_;
        enum EJsonNetReceiverEventTypes { PACKET_RECEIVED, PARTIAL_RECEIVED, PEER_CONEECTED };
        npct::common::helpers::MultiHandlerHelper<EJsonNetReceiverEventTypes, OnPacketReceiveFn, OnPartialReceiveFn, OnPeerConnectFn> mh_helper_ = {};


        asio::io_service &io_service_;
        asio::ip::tcp::acceptor acceptor_;


        void start_accept_();

        void handle_accept_(SessionClass * new_session, const asio::error_code& error);
    };

}

#include "JsonNetReceiver.tcc"

#endif // NPCT_NET_IMPL_NETRECEIVER_H