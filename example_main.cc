

#include <thread>
#include <iostream>
#include <vector>

#include <asio.hpp>

#include <cmd/cmd.h>
// #include <cmd/impl/NetPacketCommandLine.h>
#include <net/impl/JsonNetReceiver.h>

using namespace npct::net::impl;


int main(int argc, char** argv)
{
    // npct::cmd::impl::NetPacketCommandLine cmd;
    asio::io_service io_loop;
    std::vector<char> buff(512);
    JsonNetReceiver nr(io_loop, 10000);
    nr.start();
    nr.onPartialReceive([](const JsonNetReceiver* nr, Session* sess, const Session::BufferType &data)
    {
        auto e = sess->socket().remote_endpoint();
        std::stringstream ss(std::string(data.begin(), data.end()));
        std::string expr;
        while (std::getline(ss, expr, '\n')) {
            std::cout << expr;
            ss.unget();
            if (ss.get() == '\n' || ss.eof())
                std::cout << std::endl
                << fmt::format("[{}:{}] > ", e.address().to_string(), e.port());
        }

        sess->send(data);
    });
    nr.onPeerConnect([](const JsonNetReceiver* nr, Session* sess)
    {
        auto e = sess->socket().remote_endpoint();
        std::cout << fmt::format("[{}:{}] Connected.", e.address().to_string(), e.port()) << std::endl 
                  << fmt::format("[{}:{}] > ", e.address().to_string(), e.port());
    });

    nr.onPacketReceive([](const JsonNetReceiver* nr, Session* sess, const Session::PacketType *packet)
    {
        auto e = sess->socket().remote_endpoint();
        std::cout << fmt::format("[{}:{}] > {}", e.address().to_string(), e.port(), packet->to_json().dump()) << std::endl;
    });

    try
    {
        auto c_argv = const_cast<const char**>(argv+1);
        // cmd.start(std::vector<const char*>(c_argv, c_argv + (argc - 1)));
    } catch (std::runtime_error &e) {
        std::cerr << "[ERROR]! " << e.what();
    }

    std::thread thr(std::bind<decltype(std::declval<asio::io_service>().run()) (asio::io_service::*)()>(&asio::io_service::run, &io_loop));
    thr.join();
}