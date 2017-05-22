//
// Created by Raaw on 5/15/2017.
//

#ifndef NPC_IMPL_NETPACKET_MODE_H
#define NPC_IMPL_NETPACKET_MODE_H

#include <fmt/ostream.h>
#include <enum_str.h>
namespace npct::cmd::impl {
    // ENUM(NetPacketMode, size_t, SEND, RECEIVE, UNKNOWN)
    enum class NetPacketMode
    {
        SEND,
        RECEIVE,
        UNKNOWN
    };


    std::ostream &operator << (std::ostream &os, const NetPacketMode &mode);
    std::istream &operator >> (std::istream &is, NetPacketMode &mode);

}



#endif //NPC_IMPL_NETPACKET_MODE_H
