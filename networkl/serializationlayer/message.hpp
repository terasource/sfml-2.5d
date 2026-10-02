#pragma once
#include <iostream>
#include <vector>
#include <cstdint>
#include <string>
#include <bit>
#include <bitset>
#include "functional"
#include "packetwriter.hpp"
#include "packetreader.hpp"

enum class messageType : std::uint8_t {
    playerJoin = 0,
    playerInput = 1,
    playerState = 4,
    playerChat = 2,
    test_mix = 7
};

struct payloadSchemas {

};

enum class inputSnapshot {

};

class Message {
private:
    std::vector<std::uint8_t> buffer;
    //std::size_t offset = 0;
    std::uint32_t size = 0;
    std::uint32_t length = 0;
    messageType type;
    PacketWriter writer;
    PacketReader reader;
    std::string payload;
    std::vector<std::uint32_t> str_lens;

public:
    //Message();
    Message(messageType type);
    Message(const Message&) = delete;
    Message& operator =(Message&) = delete;
    void write(std::uint8_t message);
    void write(std::uint32_t message);
    void write(std::string message);
    void write(float message);

    void finalize();
    std::vector<std::uint8_t> read();
    const std::vector<std::uint8_t>& data() const;
};