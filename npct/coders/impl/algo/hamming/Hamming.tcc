#include "Hamming.h"
#include <cmath>

#if CHAR_BIT != 8
#error "CHAR_BIT IS NOT 8!"
#endif

// @see: https://github.com/mikesoylu/hamming
namespace npct::coders::impl::algo::hamming {

    template<size_t PacketBits, size_t InfoBits, typename Ch>
    bool Hamming<PacketBits, InfoBits, Ch>::getBit(Ch byte, size_t bit) {
        Ch mask = (Ch) (1 << bit);
        return byte & mask;
    }

    template<size_t PacketBits, size_t InfoBits, typename Ch>
    void Hamming<PacketBits, InfoBits, Ch>::setBit(Ch &byte, size_t bit, bool value) {
        Ch mask = (Ch) (1 << bit);
        if (value)
            byte |= mask;
        else
            byte &= ~mask;
    }

    template<size_t PacketBits, size_t InfoBits, typename Ch>
    bool Hamming<PacketBits, InfoBits, Ch>::computeParityBit(const Ch *data, int data_bits) {
        bool parity = false;
        for (size_t i = 0; i < data_bits; ++i)
            if(data[i >> (CHPow)] & 1<<(i % CHBits) > 0)
                parity = !parity;
        return parity;
    }

    template<size_t PacketBits, size_t InfoBits, typename Ch>
    bool Hamming<PacketBits, InfoBits, Ch>::calculateParityBit(
            std::array<Ch, PacketBlockSize> &block,
            size_t parity_mask)
    {
        bool parity = false;
        for(size_t i = 1; i<=PacketBits; ++i)
            if (i != parity_mask && (i & parity_mask) != 0 && getBit(block[(i-1) / CHBits], (i-1) % CHBits))
                parity = !parity;
        return parity;
    };

    template<size_t PacketBits, size_t InfoBits, typename Ch>
    std::unique_ptr<Ch[]> Hamming<PacketBits, InfoBits, Ch>::encode() {
        size_t packet_info_bit_index = 0;
        size_t packet_block_index = 0;
        size_t written_info_bit_index = 0;
        std::array<Ch, PacketBlockSize> block;
        auto result = std::make_unique<Ch[]>(encoded_size);

        auto append_data = [&] () {
            for (size_t r = 0; r<R; ++r) {
                size_t p_bit = (size_t) ((1 << r) - 1);
                setBit(block[  p_bit / CHBits], p_bit % CHBits, calculateParityBit(block, (size_t) (1 << r)));
            }
            std::memcpy(result.get()+packet_block_index*PacketBlockSize, block.data(), PacketBlockSize);
        };

        for (size_t i = 0; i < input_size; i++) {

            for (uint8_t j = 0; j < CHBits; j++, packet_info_bit_index++) {
                if (packet_info_bit_index >= InfoBits) {
                    // add parity checksum after info added
                    append_data();
                    written_info_bit_index = packet_info_bit_index = 0;
                    packet_block_index++;
                }
                while(written_info_bit_index == 0 || npct::utils::is_pow2(written_info_bit_index+1))
                    ++written_info_bit_index;
                setBit(block[written_info_bit_index / CHBits], (written_info_bit_index++) % CHBits, getBit(input[i],j));
            }
        }
        append_data();
        return std::move(result);
    }

    template<size_t PacketBits, size_t InfoBits,  typename Ch>
    std::unique_ptr<Ch[]> Hamming<PacketBits, InfoBits, Ch>::decode() {

        // fix error
        std::array<Ch, PacketBlockSize> block;
        auto result = std::make_unique<Ch[]>(decoded_size);
        size_t written_bit = 0;
        last_single_errors = 0;

        for (size_t i = 0; i<input_size; ++i) {
            std::memcpy(block.data(), input+i*PacketBlockSize, PacketBlockSize);
            size_t err_index = 0;
            for (size_t r = 0; r<R; ++r)
            {
                size_t bit = (size_t) (1 << r);
                bool parity = calculateParityBit(block, bit);
                if (parity != getBit(block[ (bit - 1) / CHBits], (bit-1) % CHBits))
                    err_index += 1 << r;

            }
            // correct error
            if (err_index != 0) last_single_errors++;
            if(err_index != 0 && err_index != npct::utils::is_pow2(err_index)) {
                size_t bit_index = (err_index-1) % CHBits;
                Ch &byte = block[(err_index-1) / CHBits];
                setBit(byte, bit_index, !getBit(byte, bit_index));
            }
            // bit copy
            for (size_t bit = 0; bit<PacketBits; ++bit) {
                if (!npct::utils::is_pow2(bit+1)) {
                    setBit(std::ref(result.get()[written_bit / CHBits]),
                           written_bit % CHBits, getBit(block[bit / CHBits], bit % CHBits));
                    written_bit++;
                }
            }
        }



        return std::move(result);
    }

}