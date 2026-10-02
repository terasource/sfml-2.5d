#pragma once
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "networkl/serializationlayer/message.hpp"
#include <poll.h>
#include <unistd.h>
#define MAX_TEMP_BUFFER_MEM 512;
#include "playerState.hpp"

class Server {
private:

public:
    std::vector<std::vector<uint8_t>> recv_all(int scket, int& bytes_processed);
    ssize_t send_all(int socket, const Message& msg);

    void print_frames(std::vector<std::vector<std::uint8_t>> frames);

    playerState calculate_playerState(const std::vector<std::uint8_t>& buffer, position& prev_char_pos);

    //returns connected file descriptor not listening socket.
    int initialize_server(int& listening_socket);
    void close_sockets(int listening_socket, int connected_socket);
    position calculate_char_pos(PacketReader& pr, position& pos);
    int send_playerstate_to_client(const unsigned int& client_socket, playerState pstate);

    void serialize_playerinput_to_playerstate();

};

