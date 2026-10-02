#pragma once
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "networkl/serializationlayer/message.hpp"
#include <unistd.h>
#include <poll.h>
#include "inputHandler.hpp"
#include "playerState.hpp"

class Client{   
    public:
        ssize_t send_all(int& socket, const Message& msg);
        std::vector<std::vector<uint8_t>> recv_all(int& scket, int& bytes_processed, int timeout);
        int initialize_client();
        void close_client(int& client_socket);
        void client_handle_input(int& client_socket);
        std::vector<std::uint8_t> read_playerstate_from_server(int& client_socket, int& bytes_received, int timeout);
        playerState deserialize_playerstate(std::vector<std::uint8_t>& buffer);
        ssize_t send_movementflag(int& client_socket, std::uint8_t movementflag);
};