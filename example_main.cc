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

#include <log/impl/Log4JLogger.h>



////////// INCLUDE YOUR CODERS HERE
#include <coders/impl/algo/crc32/CRC32Coder.h>
#include <coders/impl/algo/hamming/HammingCoder.h>
///////// END INCLUDES

using namespace npct::net::impl;
using namespace npct::coders::impl;
using namespace npct::coders::impl::algo;



void setupCodersRunner (NetCodersRunner *ncr) {
/////////// YOUR CODERS REGISTRATION GOES HERE /////////////////
    ncr->registerCoder(std::make_unique<crc32::CRC32Coder>());
    ncr->registerCoder(std::make_unique<hamming::HammingCoder>());

    /* TODO:
    // ncr->registerCoder(std::make_unique<lzw::LZWCoder>());
    // ncr->registerCoder(std::make_unique<shannon_fano::ShannonFanoCoder>());
    // ncr->registerCoder(std::make_unique<jpeg::fun::JpegConsoleViewer>());
    */

}


int main(int argc, char** argv)
{
    std::unique_ptr<npct::log::ILogger<>>
    logger = npct::log::impl::Log4JLogger::GetRoot(npct::log::LogLevel::LL_DEBUG);

    asio::io_service io_loop;
    try
    {
        auto c_argv = const_cast<const char**>(argv+1);
        std::unique_ptr<NetCodersRunner> ncr = std::make_unique<NetCodersRunner>(io_loop, 10000);
        // cmd.start(std::vector<const char*>(c_argv, c_argv + (argc - 1)));
        setupCodersRunner(ncr.get());
        ncr->start();
        std::thread thr(std::bind<decltype(std::declval<asio::io_service>().run()) (asio::io_service::*)()>(&asio::io_service::run, &io_loop));
        thr.join();
    } catch (std::runtime_error &e) {
        std::cerr << "[ERROR]! " << e.what();
    }

    return 0;
}