
#include "JsonNetReceiver.h"
#include <net/utils/utils.h>
#include <fmt/format.h>

namespace npct::net::impl
{
    using namespace std::placeholders;
    using asio::ip::tcp;
    
    void print_error(const asio::error_code& error, const tcp::endpoint &e)
    {
        std::cerr << std::endl << fmt::format("[ERROR] [{}:{}] (code: {}) > {} ", e.address().to_string(), e.port(), error.value(), error.message()) << std::endl;
    }

    ///////////////////  JSONNETRECEIVER

    void JsonNetReceiver::start()
    {
        start_accept_();
    }

    void JsonNetReceiver::onPartialReceive(const JsonNetReceiver::Base::OnPartialReceiveFn &handler)
    {
        handlers_receive_.push_back(handler);
    }

    void JsonNetReceiver::onPeerConnect(const JsonNetReceiver::Base::OnPeerConnectFn &handler)
    {
        handlers_connect_.push_back(handler);
    }

    void JsonNetReceiver::onPacketReceive(const OnPacketReceiveFn &handler)
    {
        handlers_packet_receive_.push_back(handler);
    }

    void JsonNetReceiver::emitPartialReceive(Session *sess, const Session::BufferType &d) const
    {
        for (auto &handler : handlers_receive_)
            handler(this, sess, d);
    }

    void JsonNetReceiver::emitPacketReceive(Session *sess, const Session::PacketType *packet) const
    {
        for (auto &handler : handlers_packet_receive_)
            handler(this, sess, packet);
    }

    void JsonNetReceiver::closeSession(const Session *sess)
    {
        auto sp = std::find_if(sessions_.begin(), sessions_.end(), [&](const std::unique_ptr<Session> & current) { return current.get() == sess;  });
        if (sp != sessions_.end())
            sessions_.erase(sp);
    }

    void JsonNetReceiver::emitPeerConnect(Session *sess) const
    {
        for (auto &handler : handlers_connect_)
            handler(this, sess);
    }


    void JsonNetReceiver::start_accept_()
    {
        sessions_.push_back(std::make_unique<Session>(io_service_, this));
        acceptor_.async_accept(sessions_.back()->socket(),
            std::bind(&JsonNetReceiver::handle_accept_, this, sessions_.back().get(), _1));
    }

    void JsonNetReceiver::handle_accept_(Session *new_session, const asio::error_code &error) {
        emitPeerConnect(new_session);
        auto e = new_session->socket().remote_endpoint();
        if (!error)
        {

            new_session->start();
        }
        else
            print_error(error, e);

        start_accept_();
    }

    ////////////////////   SESSION

    Session::Session(asio::io_service& io_service, JsonNetReceiver  *const ref): socket_(io_service), ref_(ref), data_(Session::DEFAULT_BUFFER_SIZE)
    {

    }

    tcp::socket& Session::socket()
    {
        return socket_;
    }

    const Session::BufferType& Session::data() const
    {
        return data_;
    }

    void Session::start()
    {
        socket_.async_read_some(asio::buffer(data_, data_.size()),
                                std::bind(&Session::handle_read, this, _1, _2));
    }

    void Session::send(const BufferType& data)
    {
        data_ = data;
        asio::async_write(socket_, asio::buffer(data_, data_.size()),
            std::bind(&Session::handle_write, this, _1));
    }

    void Session::send(const PacketType& packet)
    {
        auto raw = packet.to_raw();
        send(BufferType(raw.first.get(), raw.first.get()+raw.second));
    }

    void Session::resize_buffer(size_t new_size)
    {
        data_.resize(new_size);
    }

    size_t Session::size() const
    {
        return data_.capacity();
    }

    void Session::handle_read(const asio::error_code &error, size_t bytes_transferred) {
        auto e = socket_.remote_endpoint();
        handle_read_packet(bytes_transferred);
        ref_->emitPartialReceive(this, BufferType(data_.begin(), data_.begin()+bytes_transferred));

        if (!error)
            socket_.async_read_some(asio::buffer(data_, data_.capacity()), std::bind(&Session::handle_read, this, _1, _2));
        if (error)
        {
            if (error == asio::error::eof || error.value() == 10053)
            {
               this->ref_->closeSession(this);
            }
            else print_error(error, e);
        }
    }

    void Session::handle_read_packet(size_t bytes_transferred)
    {
        size_t offset = 0;
        while(offset < bytes_transferred)
        {
            offset += packet_buffer_.feed(data_.cbegin()+offset, bytes_transferred - offset);
            auto packet_or_null = packet_buffer_.commit();
            if (packet_or_null)
                ref_->emitPacketReceive(this, packet_or_null.get());
        }
        

    }

    void Session::handle_write(const asio::error_code &error) {
        if (error)
            print_error(error, socket_.remote_endpoint());
    }

    


    ///////////////  SESSION::BUFFER


    size_t Session::PacketBufferType::feed(const BufferType::const_iterator &source_it, size_t max_feed_size)
    {
        size_t pushed_to_buffer = std::min(bytes_left, max_feed_size);
        bytes_left -= pushed_to_buffer;
        data.insert(data.end(), source_it, source_it + pushed_to_buffer);
        return pushed_to_buffer;
    }

    std::unique_ptr<Session::PacketType> Session::PacketBufferType::commit() noexcept(false)
    {
        if (!bytes_left)
        {
            std::unique_ptr<Session::PacketType> packet_or_null = nullptr;
            switch(state)
            {
            case State::AWAIT_MORE_SIZE_HEADER_DATA: 
                std::memcpy(&bytes_left, data.data(), SIZE_HEADER_SIZE); 
                total_bytes = bytes_left = npct::net::utils::ntoh(bytes_left);
                state = State::AWAIT_MORE_DATA;
                break;
            case State::AWAIT_MORE_DATA: 
                packet_or_null = std::make_unique<Session::PacketType>(PacketType::from_raw(data.data(), total_bytes));
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
