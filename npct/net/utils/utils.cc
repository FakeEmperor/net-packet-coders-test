#include <iostream>
#include <asio/error_code.hpp>
#include <asio/ip/tcp.hpp>
#include "fmt/format.h"
#include "utils.h"

namespace npct::net::utils
{
    void print_error(const asio::error_code& error, const asio::ip::tcp::endpoint &e)
    {
       std::cerr << std::endl << fmt::format("[ERROR] [{}:{}] (code: {}) > {} ", e.address().to_string(), e.port(), error.value(), error.message()) << std::endl;
    }
}