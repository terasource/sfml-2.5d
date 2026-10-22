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
#include <random>
#include <algorithm>

class Server {
private:
    int player_id_counter = 0;
    std::vector<int> player_id_list;

public:
    std::vector<std::vector<uint8_t>> recv_all(int scket, int& bytes_processed);
    ssize_t send_all(int socket, const Message& msg);

    void print_frames(std::vector<std::vector<std::uint8_t>> frames);

    //returns connected file descriptor not listening socket.
    int initialize_server();
    void close_sockets(int listening_socket, int connected_socket);
    void handle_playerstate(Message& msg, playerState& p_state);
    int send_playerstate_to_client(const unsigned int& client_socket, playerState pstate);
    int send_playerstatevector_to_client(const unsigned int& client_socket, std::vector<playerState>& pstates);
    void serialize_playerinput_to_playerstate();
    ssize_t send_playeridtoclient(const unsigned int& client_socket, const int& client_id);
    //converts received bytes from the tcp stream to message object.
    std::vector<Message> crbs_to_msg(const std::vector<std::vector<uint8_t>>& frames_buffer);

};

