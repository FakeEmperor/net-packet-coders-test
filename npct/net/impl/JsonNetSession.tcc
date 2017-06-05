#ifndef NPCT_NET_IMPL_JSONNETSESSION_TCC
#define NPCT_NET_IMPL_JSONNETSESSION_TCC

#include <net/utils/utils.h>
#include <fmt/format.h>
#include <asio.hpp>


namespace npct::net::impl
{
    using namespace std::placeholders;
    using asio::ip::tcp;

    ////////////////////   SESSION

    template <class PacketClass_>
    JsonNetSession<PacketClass_>::JsonNetSession(asio::io_service& io_service, JsonNetReceiver<PacketClass> *const ref)
        : socket_(io_service), ref_(ref), data_(JsonNetSession<PacketClass_>::DEFAULT_BUFFER_SIZE)
    {

    }

    template <class PacketClass_>
    tcp::socket& JsonNetSession<PacketClass_>::socket()
    {
        return socket_;
    }

    template <class PacketClass_>
    const typename JsonNetSession<PacketClass_>::BufferClass& JsonNetSession<PacketClass_>::data() const
    {
        return data_;
    }

    template <class PacketClass_>
    void JsonNetSession<PacketClass_>::start()
    {
        socket_.async_read_some(asio::buffer(data_, data_.size()),
            std::bind(&JsonNetSession<PacketClass_>::handle_read, this, _1, _2));
    }

    template <class PacketClass_>
    void JsonNetSession<PacketClass_>::send(const BufferClass & data)
    {
        data_ = data;
        asio::async_write(socket_, asio::buffer(data_, data_.size()),
            std::bind(&JsonNetSession<PacketClass_>::handle_write, this, _1));
    }

    template <class PacketClass_>
    void JsonNetSession<PacketClass_>::send(const PacketClass& packet)
    {
        auto raw = packet.to_raw();
        send(BufferClass(raw.first.get(), raw.first.get() + raw.second));
    }



    template <class PacketClass_>
    void JsonNetSession<PacketClass_>::resize_buffer(size_t new_size)
    {
        data_.resize(new_size);
    }

    template <class PacketClass_>
    size_t JsonNetSession<PacketClass_>::size() const
    {
        return data_.capacity();
    }

    template <class PacketClass_>
    typename JsonNetSession<PacketClass_>::SessionSettingsClass JsonNetSession<PacketClass_>::getSettings() const
    {
        return settings_;
    }

    template <class PacketClass_>
    void JsonNetSession<PacketClass_>::setSettings(const SessionSettingsClass &settings)
    {
        settings_ = settings;
    }

    template <class PacketClass_>
    void JsonNetSession<PacketClass_>::onSessionClosing(const OnEventFn<BufferClass>& handler)
    {
        mh_helper_.addCallback(EJsonNetSessionEventType::SESSION_CLOSING, handler);
    }



    template <class PacketClass_>
    void JsonNetSession<PacketClass_>::onMessageReceived(const OnMessageFn<BufferClass>& handler)
    {
        mh_helper_.addCallback(EJsonNetSessionEventType::DATA_RECEIVED, handler);
    }

    template <class PacketClass_>
    void JsonNetSession<PacketClass_>::onMessageReceived(const OnMessageFn<PacketClass>& handler)
    {
        mh_helper_.addCallback(EJsonNetSessionEventType::PACKET_RECEIVED, handler);
    }

    template <class PacketClass_>
    void JsonNetSession<PacketClass_>::onMessageSent(const OnMessageFn<PacketClass>& handler)
    {
        mh_helper_.addCallback(EJsonNetSessionEventType::DATA_SENT, handler);
    }

    template <class PacketClass_>
    void JsonNetSession<PacketClass_>::onMessageSent(const OnMessageFn<BufferClass>& handler)
    {
        mh_helper_.addCallback(EJsonNetSessionEventType::PACKET_SENT, handler);

    }

    template <class PacketClass_>
    void JsonNetSession<PacketClass_>::handle_read(const asio::error_code &error, size_t bytes_transferred) {
        auto e = socket_.remote_endpoint();
        handle_read_packet(bytes_transferred);
        ref_->emitPartialReceive(this, BufferClass(data_.begin(), data_.begin() + bytes_transferred));

        if (!error)
            socket_.async_read_some(asio::buffer(data_, data_.capacity()), std::bind(&JsonNetSession<PacketClass_>::handle_read, this, _1, _2));
        if (error)
        {
            if (error == asio::error::eof || error.value() == 10053)
                this->ref_->closeSession(this);
            else
                utils::print_error(error, e);
        }
    }

    template <class PacketClass_>
    void JsonNetSession<PacketClass_>::handle_read_packet(size_t bytes_transferred)
    {
        size_t offset = 0;
        while (offset < bytes_transferred)
        {
            offset += packet_buffer_.feed(data_.cbegin() + offset, bytes_transferred - offset);
            auto packet_or_null = packet_buffer_.commit();
            if (packet_or_null)
                ref_->emitPacketReceive(this, packet_or_null.get());
        }


    }

    template <class PacketClass_>
    void JsonNetSession<PacketClass_>::handle_write(const asio::error_code &error) {
        if (error)
            utils::print_error(error, socket_.remote_endpoint());
    }




    ///////////////  SESSION::BUFFER

    template <class PacketClass_>
    uint32_t JsonNetSession<PacketClass_>::PacketBufferType::feed(const typename BufferClass::const_iterator &source_it, uint32_t max_feed_size)
    {
        uint32_t pushed_to_buffer = std::min(bytes_left, max_feed_size);
        bytes_left -= pushed_to_buffer;
        data.insert(data.end(), source_it, source_it + pushed_to_buffer);
        return pushed_to_buffer;
    }

    template <class PacketClass_>
    std::unique_ptr<PacketClass_> JsonNetSession<PacketClass_>::PacketBufferType::commit() noexcept(false)
    {
        if (!bytes_left)
        {
            std::unique_ptr<PacketClass_> packet_or_null = nullptr;
            switch (state)
            {
            case State::AWAIT_MORE_SIZE_HEADER_DATA:
                std::memcpy(&bytes_left, data.data(), SIZE_HEADER_SIZE);
                total_bytes = bytes_left = npct::net::utils::ntoh(bytes_left);
                state = State::AWAIT_MORE_DATA;
                break;
            case State::AWAIT_MORE_DATA:
                packet_or_null = std::make_unique<PacketClass_>(PacketClass::from_raw(data.data(), total_bytes));
                total_bytes = bytes_left = SIZE_HEADER_SIZE;
                state = State::AWAIT_MORE_SIZE_HEADER_DATA;
                break;
            }
            data.clear();
            return packet_or_null;
        }
        return nullptr;
    }
}


#endif //NPCT_NET_IMPL_JSONNETSESSION_TCC