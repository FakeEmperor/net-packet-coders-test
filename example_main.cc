#include <thread>
#include <iostream>
#include <vector>
#include <asio.hpp>
#include <fmt/format.h>


#include <common/callbacks.h>
#include <csignal>

// #include <cmd/cmd.h>
// #include <cmd/impl/NetPacketCommandLine.h>
// #include <data/impl/NTestPacket.h>
#include <utils/utils.h>
#include <net/impl/JsonNetReceiver.h>
#include <net/impl/JsonNetSession.h>
#include <coders/impl/NetCodersRunner.h>

// LOG SOLUTION
#ifdef NPCT_LOG_ENABLE_LOG4CPP
#include <log/impl/Log4JLogger.h>
using LoggerImpl = npct::log::impl::Log4JLogger;
constexpr bool CONFIGURE_FROM_FILE_ENABLED = true;
#else
#include <log/impl/GenericLogger.h>
using LoggerImpl = npct::log::impl::GenericLogger;
constexpr bool CONFIGURE_FROM_FILE_ENABLED = false;
#endif



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

    hamming::HammingCoder<7,4>::SetLogger(
            std::make_shared<LoggerImpl>("hamming", npct::log::LogLevel::LL_DEBUG, true)
    );
    ncr->registerCoder(std::make_unique<hamming::HammingCoder<7,4>>());

    /* TODO:
    // ncr->registerCoder(std::make_unique<lzw::LZWCoder>());
    // ncr->registerCoder(std::make_unique<shannon_fano::ShannonFanoCoder>());
    // ncr->registerCoder(std::make_unique<jpeg::fun::JpegConsoleViewer>());
    */

}




#ifdef WIN32
#include <Windows.h>

BOOL WINAPI handle_sigint(DWORD signal) {
    std::ostringstream os;
    os << std::this_thread::get_id();
    if (signal == CTRL_C_EVENT)
        throw std::runtime_error(fmt::format("{} Received CTRL-C signal!", os.str()));

    return TRUE;
}
void setupInterrupts() {
    if (!SetConsoleCtrlHandler(handle_sigint, TRUE))
        throw std::runtime_error("Could not setup Windows CTRL-C handler");
}

#else
void handle_sigint (int) {
    throw std::runtime_error("Received CTRL-C signal!");
};
void setupInterrupts() {

    signal (SIGINT, handle_sigint);
}
#endif

int main(int argc, char** argv)
{
    setupInterrupts();

    if(CONFIGURE_FROM_FILE_ENABLED)
        LoggerImpl::configureFromFile("./configs/log4cpp.properties");

    auto logger = LoggerImpl::GetRoot(npct::log::LogLevel::LL_DEBUG);
    logger->log(npct::log::LogLevel::LL_INFO, "Initializing server...");

    asio::io_service io_loop;
    try
    {
        auto c_argv = const_cast<const char**>(argv+1);
        std::unique_ptr<NetCodersRunner> ncr = std::make_unique<NetCodersRunner>(io_loop, 10000);
        // cmd.start(std::vector<const char*>(c_argv, c_argv + (argc - 1)));
        logger->log(npct::log::LogLevel::LL_INFO, "Initialized!");
        setupCodersRunner(ncr.get());
        logger->log(npct::log::LogLevel::LL_INFO, "Starting server...");
        ncr->start();
        std::thread thr(std::bind<decltype(std::declval<asio::io_service>().run()) (asio::io_service::*)()>(&asio::io_service::run, &io_loop));
        logger->log(npct::log::LogLevel::LL_INFO, "Ready for connections!");
        thr.join();
    } catch (std::runtime_error &e) {
        logger->log(npct::log::LogLevel::LL_ERROR, "[CRITICAL]!! %s", e.what());
    }

    return 0;
}