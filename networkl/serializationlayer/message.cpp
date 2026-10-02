#include "message.hpp"

/*
    [length][type][payload] is the framing structure.
        length = the length of the message in payload not the size.
        type = the type of the payload
        payload = the message itself

        size is seperately defined as class member but not placed in frame.
    
    consider framing as [type][length][payload] idk just sounds more logical to me.
*/

/*
    max_frame_size = 512b; // for now
    max_payload_size = max_frame_size - 4[length] - 1[type] = 507b;

    payload schemas:
    general schema = [[length][type][payload]]
    playerJoin = [[timeStamp][playerName][joinMessage]];
    playerInput = [[playerID][playerState][inputFlags]];
    playerState = [[playeriD][player:position:x][player:position:y]];

*/

// implement a write_playerstate message to frame its payload and send it from server to client and read.

Message::Message(messageType type) : writer(buffer), reader(buffer){
    // backpatching
    writer.write_uint32(length);

    this->type = type;
    writer.write_uint8(static_cast<uint8_t>(type));
    //std::cout << "type at constructor is = " << reinterpret_cast<const uint8_t*>(typeid(type).name()) << std::endl;

}
//uint8_t overload
void Message::write(std::uint8_t message){

    writer.write_uint8(message);
} 

//uint32_t overload
void Message::write(std::uint32_t message){

    writer.write_uint32(message);
} 

// string overload
void Message::write(std::string message){

    // maybe later i can map this message length with the message itself but i need to check tradeoffs
    str_lens.push_back(message.length());
    // enum class has been defined as a uint8_t aswell idk why i had to need cast it to again.
    writer.write_string(message);
} 


//float overload
void Message::write(float message){

    //std::uint32_t bits_to_write = std::__bit_cast<std::uint32_t>(message);
    //writer.write_uint32(bits_to_write);

    writer.write_float(message);
} 


std::vector<std::uint8_t> Message::read(){

    switch(type){
        case messageType::test_mix:
        std::cout 
        << "[" << reader.read_uint32() << "] " 
        << "[" << static_cast<int>(reader.read_uint8()) << "] "
        << "[" << static_cast<int>(reader.read_uint8()) << "] "
        << "[" << reader.read_uint32() << "] "
        << "["  << reader.read_string(4) << "]" 
        << std::endl;

        return buffer;
        break;

        case messageType::playerInput:
        std::cout 
        << "[length: " << reader.read_uint32() << "]"
        << "[type: " << static_cast<int>(reader.read_uint8()) << "]" 
        << "[payload: " << std::bitset<8> (reader.read_uint8()) << "]" << std::endl;

        return buffer;
        break;

        case messageType::playerState:
        std::cout 
        << "[length: " << reader.read_uint32() << "]"
        << "[type: " << static_cast<int>(reader.read_uint8()) << "]"
        << "[payload: " << std::endl;
    }

    return buffer;
}

// i actually dont need this as a seperate function i guess but,
// probably this will be better solution then implemented rn.
// my current solution is to calculate the current payload size in every writer.write function and then write it to buffer.
// but this takes a lot processes. actually its better to prefer calculate and write it at once than doing it at every step.
void Message::finalize(){

    //maybe i can include the type to a length in the future but now i dont want it to.
    length = buffer.size() - sizeof(type) - sizeof(length);

    writer.write_uint32_at(0, length);

    size = buffer.size() * sizeof(uint8_t);
    
    // message size 4byte + 1byte + how many bytes is the message are.
    // but the packet will be serialized as length(of message) + type + message
}


const std::vector<std::uint8_t>& Message::data() const{
    return buffer;
}
