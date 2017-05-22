#ifndef NPCT_NET_IMPL_NETRECEIVER_H
#define NPCT_NET_IMPL_NETRECEIVER_H

#include <json.hpp>

#include <data/impl/NTestPacket.h>
#include <net/IPacketReceiver.h>
#include <net/IDataReceiver.h>

#include <asio/io_service.hpp>
#include <asio/ip/tcp.hpp>
#include <asio.hpp>


namespace npct::net::impl
{

    using asio::ip::tcp;

    class JsonNetReceiver;

    class Session
    {
    public:
        typedef std::vector<char> BufferType;
        typedef data::impl::NTestPacket PacketType;


        static constexpr size_t DEFAULT_BUFFER_SIZE = 512;

        Session(asio::io_service& io_service, JsonNetReceiver *const  ref);

        tcp::socket& socket();

        const BufferType& data() const;

        void start();

        void send(const BufferType& data);
        void send(const PacketType& packet);

        void resize_buffer(size_t new_size);

        size_t size() const;

    private:
        void handle_read(const asio::error_code& error, size_t bytes_transferred);

        void handle_read_packet(size_t bytes_transferred);

        void handle_write(const asio::error_code& error);


        tcp::socket socket_;
        BufferType data_;
        struct PacketBufferType
        {
            static constexpr size_t SIZE_HEADER_SIZE = sizeof(size_t);
            enum class State { AWAIT_MORE_SIZE_HEADER_DATA, AWAIT_MORE_DATA } state = { State::AWAIT_MORE_SIZE_HEADER_DATA };
            size_t bytes_left = { SIZE_HEADER_SIZE };
            size_t total_bytes = { };
            BufferType data;
            
            size_t feed(const BufferType::const_iterator &source_it, size_t max_feed_size);
            std::unique_ptr<PacketType> commit() noexcept(false);
        } packet_buffer_;


        JsonNetReceiver *const ref_;
    };

    class JsonNetReceiver: IPacketReceiver<JsonNetReceiver, Session>
    {
    public:
        typedef IPacketReceiver<JsonNetReceiver, Session> Base;

        JsonNetReceiver(asio::io_service &io_loop, size_t port, bool debug_ = false) : 
            io_service_(io_loop), acceptor_(io_loop, tcp::endpoint(tcp::v4(), port)) {}

        void start() override;

        void onPartialReceive(const Base::OnPartialReceiveFn& handler) override;
        void onPeerConnect(const Base::OnPeerConnectFn& handler) override;
        void onPacketReceive(const OnPacketReceiveFn&) override;

        void emitPartialReceive(Session*, const Session::BufferType &) const;
        void emitPacketReceive(Session*, const  Session::PacketType*) const;
        void closeSession(const Session*) override;
        void emitPeerConnect(Session*) const;
    private:
        std::vector<std::unique_ptr<Session>> sessions_;
        std::vector<Base::OnPacketReceiveFn> handlers_packet_receive_;
        std::vector<Base::OnPartialReceiveFn> handlers_receive_;
        std::vector<Base::OnPeerConnectFn> handlers_connect_;

        asio::io_service& io_service_;
        tcp::acceptor acceptor_;


        void start_accept_();

        void handle_accept_(Session* new_session, const asio::error_code& error);
    };

}



#endif // NPCT_NET_IMPL_NETRECEIVER_H