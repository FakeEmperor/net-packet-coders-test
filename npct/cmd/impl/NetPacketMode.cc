//
// Created by Raaw on 5/17/2017.
//

#include "NetPacketMode.h"
#include <algorithm>

namespace npct::cmd::impl {
    std::ostream &operator << (std::ostream &os, const NetPacketMode &mode) {
        // os << (mode._to_string());
        std::string ms;
        switch(mode)
        {
        case NetPacketMode::SEND: ms = "SEND";       break;
        case NetPacketMode::RECEIVE: ms = "RECEIVE"; break;
        case NetPacketMode::UNKNOWN: ms = "UNKNOWN"; break;
        default: ;
        }

        return os << ms;
    }

	std::istream& operator>>(std::istream& is, NetPacketMode& mode)
	{
		std::string name; 
		is >> name; std::transform(name.begin(), name.end(), name.begin(), ::tolower);
		// mode = NetPacketMode::_from_string_i(name.c_str());
        if (name == "send") mode = NetPacketMode::SEND;
        else if (name == "receive") mode = NetPacketMode::RECEIVE;
        else if (name == "unknown") mode = NetPacketMode::UNKNOWN;
        else throw std::runtime_error("unknown mode");


		return is;
	}
}
