

#include <thread>
#include <iostream>
#include <vector>
#include <asio.hpp>
#include <fmt/format.h>


#include <common/callbacks.h>
// #include <cmd/cmd.h>
// #include <cmd/impl/NetPacketCommandLine.h>
// #include <data/impl/NTestPacket.h>
#include <utils/utils.h>
#include <net/impl/JsonNetReceiver.h>
#include <net/impl/JsonNetSession.h>
#include <coders/impl/NetCodersRunner.h>
// ALGORITHMS
#include <coders/impl/algo/CRC32Coder.h>

using namespace npct::net::impl;
using namespace npct::coders::impl;
using namespace npct::data::impl;


int main(int argc, char** argv)
{

    // npct::cmd::impl::NetPacketCommandLine cmd;
    asio::io_service io_loop;

    // JsonNetReceiver<NTestPacket> nr(io_loop, 10001);
    NetCodersRunner ncr(io_loop, 10000);
    ncr.registerCoder(std::make_unique<algo::CRC32Coder>());
    ncr.start();
//
//    nr.start();
//    nr.onPartialReceive([](const JsonNetReceiver<NTestPacket> * nr, JsonNetSession<NTestPacket> * sess, const typename JsonNetSession<NTestPacket>::BufferClass &data)
//    {
//        auto e = sess->socket().remote_endpoint();
//        std::stringstream ss(std::string(data.begin(), data.end()));
//        std::string expr;
//        while (std::getline(ss, expr, '\n')) {
//            std::cout << expr;
//            ss.unget();
//            if (ss.get() == '\n' || ss.eof())
//                std::cout << std::endl
//                << fmt::format("[{}:{}] > ", e.address().to_string(), e.port());
//        }
//
//        sess->send(data);
//    });
//    nr.onPeerConnect([](const JsonNetReceiver<NTestPacket>* nr, JsonNetSession<NTestPacket>* sess)
//    {
//        auto e = sess->socket().remote_endpoint();
//        std::cout << fmt::format("[{}:{}] Connected.", e.address().to_string(), e.port()) << std::endl
//                  << fmt::format("[{}:{}] > ", e.address().to_string(), e.port());
//    });
//
//    nr.onPacketReceive([](const JsonNetReceiver<NTestPacket> * nr, JsonNetSession<NTestPacket> * sess, const typename JsonNetSession<NTestPacket>::PacketClass *packet)
//    {
//        auto e = sess->socket().remote_endpoint();
//        std::cout << fmt::format("[{}:{}] > {}", e.address().to_string(), e.port(), packet->to_json().dump()) << std::endl;
//    });

    try
    {
        auto c_argv = const_cast<const char**>(argv+1);
        // cmd.start(std::vector<const char*>(c_argv, c_argv + (argc - 1)));
    } catch (std::runtime_error &e) {
        std::cerr << "[ERROR]! " << e.what();
    }

    std::thread thr(std::bind<decltype(std::declval<asio::io_service>().run()) (asio::io_service::*)()>(&asio::io_service::run, &io_loop));
    thr.join();
    return 0;
}