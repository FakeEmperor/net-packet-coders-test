//
// Created by lol on 6/5/2017.
//

#ifndef NPCT_CODERS_IMPL_ALGO_HAMMING_HAMMING_H
#define NPCT_CODERS_IMPL_ALGO_HAMMING_HAMMING_H

#include <utils/utils.h>
#include <memory>

namespace npct::coders::impl::algo::hamming {

    // @see: https://github.com/mikesoylu/hamming
    template<size_t PacketBits, size_t InfoBits, typename Ch = char>
    class Hamming {
        static constexpr uint8_t R = npct::utils::msb_bit_order(PacketBits+1);

        static_assert(R == (uint8_t)(-1) || R == npct::utils::msb_bit_order(InfoBits+1+R),
                      "Wrong number of 'PacketBits' or 'InfoBits'. Are you sure you have "
                      "'PacketBits' like (2^r)-1 and 'InfoBits' like (2^r)-r-1?");


        // gets the value of a parity bit at position bit which IS_POWER_OF_TWO

    public:
        static constexpr uint8_t CHPow = 3*sizeof(Ch);
        static constexpr uint8_t CHBits = CHAR_BIT*sizeof(Ch);
        static constexpr uint8_t PacketBlockSize = (uint8_t) ((R + (CHPow - R % CHPow) % CHPow) / CHPow); // size in bytes

        Hamming(const Ch *packet, size_t packet_length)
            : input(packet), input_size(packet_length),
              encoded_size((packet_length * CHBits) / InfoBits * PacketBlockSize + ( (packet_length*CHBits)% InfoBits ? PacketBlockSize : 0) ),
              decoded_size(packet_length / PacketBlockSize * InfoBits / CHBits)
        { };

        // encodes packet of length getEncodedLength
        std::unique_ptr<Ch[]> encode();

        // decodes a packet of length getDecodedLength
        std::unique_ptr<Ch[]> decode();

        static bool computeParityBit(const Ch *data, int data_bits);
        static void setBit(Ch &byte, size_t bit, bool value);
        static bool getBit(Ch byte, size_t bit);

        bool calculateParityBit(std::array<Ch, PacketBlockSize> &block, size_t parity_mask);
        const size_t input_size;
        const size_t encoded_size;
        const size_t decoded_size;
        size_t last_single_errors = { 0 };
        const Ch *input = { nullptr };

    };

}
#include "Hamming.tcc"

#endif //NPCT_CODERS_IMPL_ALGO_HAMMING_HAMMING_H
