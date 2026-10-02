#include "packetreader.hpp"

// how the reader will know which type of byte to read if i dont hardcode the order of reading bytes manually?


// embed finalize function into the deconstructor in order not to forget call it xd. 
PacketReader::PacketReader(const std::vector<std::uint8_t>& data) : buffer(data) {

}

std::uint8_t PacketReader::read_uint8() {
    // the intention is the return offset index of buffer which we present buffer in network order layer where
    // the vector is aa bb cc dd and we want to read from start to end so.
    // returns the offset index and increase offset by 1.

    return buffer[offset++];
}

// this function was for reading the type of message in finalize() function on message class but i found a better
// solution so now no needed but maybe later i would use idk so ill leave it.
std::uint8_t PacketReader::read_uint8_at(size_t index) {

    // too bad casting int to uint8_t this is not going to work
    return buffer[index];
}

std::uint32_t PacketReader::read_uint32() {

    std::uint32_t return_buff = 0;
    int len = sizeof(uint32_t) / sizeof(uint8_t);

    // the idea is adding the most significant bit first and shift it to left by 1 byte and then add the next one.
    // return_buff = 0; res = return_buff + buffer[offset(0 at beggining or some value idk)],
    // lets say buffer[offset] is equal to 0xAA now, = 0xAA. and res << 8, res = 0xAA00 and now we can
    // add 0xBB to it to get 0xAABB.
    for (size_t i = offset; i < offset + len; i++) {
        // at first i wrote return_buff += buffer[offset++] << 8; but this is probably wrong implementation of my idea on above
        // cause it will add buffer[offset++] to return_buff but actually i doesnt add buffer[offset++] it adds buffer[offset++] << 8
        // which is if we accept buffer[offset++] as 0xAA and the statement will produce result as 0xAA00 and will add this to return_buff.
        // but what i want was add buffer[offset++] and then shift the produced result.
        uint8_t next_byte = buffer[i];

        return_buff = (return_buff << 8) | next_byte;
    }

    // but. now this function produces not uint32_t but uint40_t which is 0xAABBCCDD00.
    // thats because we shift 4 times but we need 3. if you want to cut a stick into 4 pieces you will need 3 slices not 4.
    // and so i think there is no need to modify the for loop or logic. i just can mask the last 0x00.
    // with shifting the all result right by 8 bits.

    offset += len;
    return return_buff;
}

//starts from index and reads 4 bytes through buffer/
std::uint32_t PacketReader::read_uint32_at(std::size_t index) {

    std::uint32_t return_buff = 0;
    int uplimit = sizeof(uint32_t) / sizeof(uint8_t);
    for (std::size_t i = index; i < index + uplimit; i++) {
        std::uint8_t next_byte = buffer[i];

        return_buff = (return_buff << 8) | next_byte;
    }

    return return_buff;
}



std::string PacketReader::read_string(size_t len) {
    std::string str_buff;

    for (size_t i = offset; i < offset + len; i++) {
        str_buff += static_cast<char>(buffer[i]);
    }

    offset += len;
    return str_buff;
}


float PacketReader::read_float() {

    uint32_t read_buff = read_uint32();
    float float_buff = std::bit_cast<float>(read_buff);

    return float_buff;
}

int PacketReader::get_offset() {
    return offset;
}
