#pragma once
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "networkl/serializationlayer/message.hpp"
#include <unistd.h>
#include <poll.h>
#include <ctime>
#include "inputHandler.hpp"
#include "player/playerState.hpp"
#include "algorithm"

class Client {
public:
    ssize_t send_all(int& socket, const Message& msg);
    ssize_t send_movementflag(int& client_socket, std::uint8_t movementflag);
    std::vector<std::vector<uint8_t>> recv_all(int& scket, int& bytes_processed, int timeout);

    //converts received bytes from the tcp stream to message object.
    std::vector<Message> crbs_to_msg(const std::vector<std::vector<std::uint8_t>>& frames_buffer);

    int initialize_client();
    void close_client(int& client_socket);
    void client_handle_input(int& client_socket);

    int get_player_id(const std::vector<Message>& msgs);
    std::vector<playerState> deserialize_playerstates(std::vector<Message>& msg_buffer, std::vector <playerState>& player_state_list);

};