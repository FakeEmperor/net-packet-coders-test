

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





////////// INCLUDE YOUR CODERS HERE
#include <coders/impl/algo/crc32/CRC32Coder.h>
///////// END INCLUDES

using namespace npct::net::impl;
using namespace npct::coders::impl;
using namespace npct::coders::impl::algo;



std::unique_ptr<NetCodersRunner> setupCodersRunner (uint16_t port, asio::io_service &io_loop) {
    std::unique_ptr<NetCodersRunner> ncr = std::make_unique<NetCodersRunner>(io_loop, port);

/////////// YOUR CODERS REGISTRATION GOES HERE /////////////////
    ncr->registerCoder(std::make_unique<crc32::CRC32Coder>());


    return std::move(ncr);
}


int main(int argc, char** argv)
{
    asio::io_service io_loop;
    try
    {
        auto c_argv = const_cast<const char**>(argv+1);
        // cmd.start(std::vector<const char*>(c_argv, c_argv + (argc - 1)));
        auto ncr = setupCodersRunner(10000, io_loop);
        ncr->start();
        std::thread thr(std::bind<decltype(std::declval<asio::io_service>().run()) (asio::io_service::*)()>(&asio::io_service::run, &io_loop));
        thr.join();
    } catch (std::runtime_error &e) {
        std::cerr << "[ERROR]! " << e.what();
    }

    return 0;
}