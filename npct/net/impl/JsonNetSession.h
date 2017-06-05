#ifndef NPCT_NET_IMPL_JSONNETSESSION_H
#define NPCT_NET_IMPL_JSONNETSESSION_H

#include <asio/ip/tcp.hpp>
#include <common/helpers/handler_helpers.h>

namespace npct::net::impl
{
    using asio::ip::tcp;

    template<class>
    class JsonNetReceiver;

    template <class PacketClass_>
    class JsonNetSession : 
    public IPeerSession<JsonNetSession<PacketClass_>, BaseSessionSettings, std::vector<char>>,
    public IPeerSession<JsonNetSession<PacketClass_>, BaseSessionSettings, PacketClass_> {
    public:
        using DataBasedSession = IPeerSession<JsonNetSession, BaseSessionSettings, std::vector<char>>;
        using PacketBasedSession = IPeerSession<JsonNetSession, BaseSessionSettings, PacketClass_>;

        using SessionSettingsClass = typename DataBasedSession::SessionSettingsClass;

        // BUG: why is that? Well, fuck you, MSVC. Clang, GCC compiles it without this fucking crutch.
        template <typename T>
        using DataClass = typename IPeerSession<JsonNetSession, BaseSessionSettings, T>::MessageClass;
        template <typename T>
        using OnEventFn = typename IPeerSession<JsonNetSession, BaseSessionSettings, T>::OnEventFn;
        template <typename T>
        using OnMessageFn = typename IPeerSession<JsonNetSession, BaseSessionSettings, T>::OnMessageFn;


        using BufferClass = DataClass<std::vector<char>>;
        using PacketClass = DataClass<PacketClass_>;

        static constexpr size_t DEFAULT_BUFFER_SIZE = 512;

        JsonNetSession(asio::io_service &io_service, JsonNetReceiver<PacketClass> * const ref);

        tcp::socket &socket();

        const BufferClass &data() const;

        void start();

        void send(const BufferClass &data) override;

        void send(const PacketClass &packet) override;

        void resize_buffer(size_t new_size);

        size_t size() const;


        SessionSettingsClass getSettings() const override;

        void setSettings(const SessionSettingsClass &) override;

        void onSessionClosing(const OnEventFn<BufferClass> &handler) override;

        void onMessageReceived(const OnMessageFn<PacketClass> &handler) override;

        void onMessageReceived(const OnMessageFn<BufferClass>  &handler) override;

        void onMessageSent(const OnMessageFn<PacketClass>  &handler) override;

        void onMessageSent(const OnMessageFn<BufferClass>  &handler) override;
    private:
        // message queues
        enum EJsonNetSessionEventType { SESSION_CLOSING, DATA_RECEIVED, DATA_SENT, PACKET_RECEIVED, PACKET_SENT };
        common::helpers::MultiHandlerHelper<EJsonNetSessionEventType,
                typename DataBasedSession::OnEventFn, typename DataBasedSession::OnMessageFn, typename PacketBasedSession::OnMessageFn> mh_helper_;


        void handle_read(const asio::error_code& error, size_t bytes_transferred);

        void handle_read_packet(size_t bytes_transferred);

        void handle_write(const asio::error_code& error);

        SessionSettingsClass settings_;
        tcp::socket socket_;
        BufferClass data_;

        struct PacketBufferType
        {
            static constexpr uint32_t SIZE_HEADER_SIZE = sizeof(uint32_t);
            enum class State { AWAIT_MORE_SIZE_HEADER_DATA, AWAIT_MORE_DATA } state = { State::AWAIT_MORE_SIZE_HEADER_DATA };
            uint32_t bytes_left = { SIZE_HEADER_SIZE };
            uint32_t total_bytes = {};
            BufferClass data;

            uint32_t feed(const typename BufferClass::const_iterator &source_it, uint32_t max_feed_size);
            std::unique_ptr<PacketClass_> commit() noexcept(false);
        } packet_buffer_;


        JsonNetReceiver<PacketClass_> *const ref_;
    };

 }

#include "JsonNetSession.tcc"


#endif // !NPCT_NET_IMPL_JSONNETSESSION_H
