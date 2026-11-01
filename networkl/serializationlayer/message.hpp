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
#include "player/playerState.hpp"


struct payloads {

    struct playerJoin {
        uint32_t timestamp;
        uint8_t id;
        float x;
        float y;
        std::string joinMessage;
    };

    struct playerInput {
        uint8_t inputflag;
    };

};



enum class messageType : std::uint8_t {
    playerID = 5,
    playerJoin = 0,
    playerInput = 1,
    playerState = 4,
    playerChat = 2,
    test_mix = 7
};


class Message {
private:
    std::vector<std::uint8_t> buffer;

    std::uint32_t size = 0;

    std::uint32_t length = 0;
    messageType _type;

    PacketWriter writer;
    PacketReader reader;
    std::string payload;
    std::vector<std::uint32_t> str_lens;

public:
    //Message();
    Message(messageType type);

    //look about this after
    //Message(const Message&) = delete;
    Message& operator =(Message&) = delete;
    void write(std::uint8_t message);
    void write(std::uint32_t message);
    void write(std::string message);
    void write(float message);

    void finalize();
    Message read();
    playerState readps();
    payloads::playerInput readpi();
    payloads::playerJoin readpj();
    uint8_t readpid();
    void print();
    const std::vector<std::uint8_t>& data() const;
    const messageType& type() const;

};