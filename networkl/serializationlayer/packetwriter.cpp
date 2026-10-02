#include "packetwriter.hpp"

PacketWriter::PacketWriter(std::vector<std::uint8_t>& data) : buffer(data) {

}

void PacketWriter::write_uint8(std::uint8_t value) {
        buffer.push_back(value);
}

void PacketWriter::write_uint32(std::uint32_t value) {

        // to write uint32 as network byte order(big endian which the the most significant(leftmost) bit is passed first)
        // we need to right shift the value in reverse order in order to get leftmost bit first.

        // for example value = 0xAABBCCDD where we search 0xAA as the most significant bit and to push_back into buffer.
        // we have 32 bits there as every number which represented with letters like A ,B ,C and D are 4 bits,
        // and we are looking for 0xAA which are 8 bits.
        // to mask 0xAA we need it to be moved to rightmost and to get this we shift the value 24 bits right. 
        // which in results 0xAA

        int limit = sizeof(std::uint32_t) / sizeof(std::uint8_t);

        for (int i = limit - 1; i >= 0; i--) {
                // 0xFF is here for pushing the last 8 bits back.
                std::uint8_t leftmost = (value >> i * 8) & 0xFF;
                buffer.push_back(leftmost);
        }

        // 0xAABBCCDD should be pushed back into buffer as [0xAA],[0xBB],[0xCC],[0xDD] which is called as network byte order.
        // this is not the best optimized solution but for now i think it should be decent.
        // i found some O(1) solution on net but just could not figure it out what is going on there.
        // solution was:
        // #define REV(x) ( ((x&0xff000000)>>24) | (((x&0x00ff0000)<<8)>>16) | (((x&0x0000ff00)>>8)<<16) | ((x&0x000000ff) << 24) )
        // but looks so complicated for me to understand this for now.
}

// starts from index and writes 4 indices. and overwrites if the index is occupied before.
void PacketWriter::write_uint32_at(int index, std::uint32_t value) {
        int limit = sizeof(std::uint32_t) / sizeof(std::uint8_t);

        for (int i = limit - 1; i >= 0; i--) {
                // 0xFF is here for pushing the last 8 bits back.
                std::uint8_t leftmost = (value >> i * 8) & 0xFF;
                //overwrites
                buffer.at(index + limit - 1 - i) = leftmost;
        }
}

void PacketWriter::write_string(const std::string& value) {

        for (int i = 0; i < value.size(); i++)
                // where a char takes 8 bits. it can be casted as uint8_t.
                buffer.push_back(static_cast<uint8_t>(value[i]));
}

// float packages can be written as uint32 and be read as float 
// so there is nothing to implement differently from read_uint32
// but the read method is going to be changed.

void PacketWriter::write_float(float value) {
        write_uint32(std::bit_cast<std::uint32_t>(value));
}



const std::vector<std::uint8_t>& PacketWriter::data() const {

        const std::vector<std::uint8_t>& result = buffer;


        return result;
}

