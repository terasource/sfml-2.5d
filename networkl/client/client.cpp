#include "client.hpp"


ssize_t Client::send_all(int& socket, const Message& msg) {
    ssize_t bytes_to_send = msg.data().size();

    const std::vector<std::uint8_t>& buffer = msg.data();

    ssize_t total_bytes_sent = 0;

    while (total_bytes_sent < bytes_to_send) {

        ssize_t bytes_sent = send(socket, buffer.data() + total_bytes_sent, bytes_to_send - total_bytes_sent, 0);

        if (bytes_sent == -1) {
            perror("sent");
            return -1;
        }

        total_bytes_sent += bytes_sent;
    }

    return total_bytes_sent;
}

//shrink this function into only to return std::vector not nested vector.

//receives the stream as bytes from tcp.
std::vector<std::vector<uint8_t>> Client::recv_all(int& scket, int& bytes_processed, int timeout) {
    std::vector<std::vector<uint8_t>> frames;
    std::vector<uint8_t> frame;
    std::vector<uint8_t> temp_buffer(512);
    size_t bytes_to_processed = 0;
    int total_bytes_processed = 0;
    PacketReader pr(temp_buffer);

    while (true) {
        pollfd pfd;
        pfd.fd = scket;
        pfd.events = POLLIN;
        int ret = poll(&pfd, 1, timeout);

        if (ret < 0) {
            perror("poll");
            break;
        }

        if (ret == 0)
            break;

        if (pfd.revents & POLLIN) {
            int received = recv(scket, temp_buffer.data() + bytes_to_processed, 512 - bytes_to_processed, 0);

            //in order not to overwrite the 0.5 frame in 1.5 frame and etc.
            if (received > 0)
                bytes_to_processed += received;


            //what if i takes bytes in range 0-3. program will be looped forever.
            while (bytes_to_processed >= 4) {
                size_t frame_length = (pr.read_uint32_at(0) + 4 + 1);

                if (bytes_to_processed < frame_length)
                    break;

                if (bytes_to_processed >= frame_length) {
                    frame.insert(frame.begin(), temp_buffer.begin(), temp_buffer.begin() + frame_length);
                    temp_buffer.erase(temp_buffer.begin(), temp_buffer.begin() + frame_length);
                    temp_buffer.resize(temp_buffer.size() + frame_length);
                    bytes_to_processed -= frame_length;
                    total_bytes_processed += frame_length;
                }

                if (frame.size() == frame_length) {
                    frames.push_back(frame);
                    frame.clear();
                }

            }

            if (received == 0) {
                //std::cout << "frame bitti mk gelmiyo dahaaaa " << std::endl;
                close(scket);
                break;
            }

            if (received == -1) {
                perror("recv");
                std::exit(1);
            }

            //std::cout << "received bytes = " << total_received_bytes << " remaining bytes = " << bytes_to_receive << std::endl;  
        }

    }

    bytes_processed = total_bytes_processed;
    return frames;
}

std::vector<Message> Client::crbs_to_msg(const std::vector<std::vector<std::uint8_t>>& frames_buffer) {

    std::vector<Message> msg_vector;

    for (size_t i = 0; i < frames_buffer.size(); i++) {

        PacketReader pr(frames_buffer[i]);
        messageType msg_type = static_cast<messageType>(pr.read_uint8_at(4));
        Message msg(msg_type);

        switch (msg_type) {
        case messageType::playerJoin: {
            pr.read_uint32(); //length
            pr.read_uint8();  //type
            uint8_t id = pr.read_uint8();  //id
            //uint32_t timestamp = pr.read_uint32(); //timestamp
            //playername and joinmessage will be decided by client.

            msg.write(id);
            //msg.write(timestamp);
            msg.finalize();

            break;
        }

        case messageType::playerInput: {
            pr.read_uint32(); //length
            pr.read_uint8();  //type
            uint8_t inputflags = pr.read_uint8();  //inputflags

            msg.write(inputflags);
            msg.finalize();
            break;

        }

        case messageType::playerChat: {
            //later
            break;
        }

        case messageType::playerState: {
            pr.read_uint32(); //length
            pr.read_uint8();  //type
            uint8_t id = pr.read_uint8();  //id
            float pos_x = pr.read_float();  //position:x
            float pos_y = pr.read_float();  //position:y

            msg.write(id);
            msg.write(pos_x);
            msg.write(pos_y);
            msg.finalize();
            break;
        }

        case messageType::playerID: {
            pr.read_uint32(); //length
            pr.read_uint8();  //type
            uint8_t id = pr.read_uint8();  //id

            msg.write(id);
            msg.finalize();

            break;
        }

        case messageType::test_mix: {

            break;
        }

        }
        msg_vector.push_back(msg);
    }

    return msg_vector;
}

int Client::initialize_client() {
    addrinfo hints{};
    addrinfo* server_info;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    // skip the ai_protocol AI_PASSIVE here because this is client not a server accepts a connection.

    getaddrinfo("127.0.0.1", "1280", &hints, &server_info);

    int client_socket = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

    if (client_socket == -1)
        perror("socket");

    connect(client_socket, server_info->ai_addr, server_info->ai_addrlen);

    return client_socket;
}

void Client::close_client(int& client_socket) {

    shutdown(client_socket, SHUT_WR);

    close(client_socket);
}

int Client::get_player_id(const std::vector<Message>& msgs) {

    uint8_t p_id = 0;

    for (Message msg : msgs) {
        if (msg.type() == messageType::playerID) {
            p_id = msg.readpid();
        }
    }

    return p_id;
}

void Client::client_handle_input(int& client_socket) {
    inputHandler inputhandler;
    std::uint8_t movementFlag = inputhandler.collectInput();
    Message input(messageType::playerInput);
    input.write(movementFlag);
    input.finalize();

    int sent_all = send_all(client_socket, input);
    if (sent_all < 0)
        return;

    input.print();
}

ssize_t Client::send_movementflag(int& client_socket, std::uint8_t movementflag) {

    Message msg_player_input(messageType::playerInput);
    msg_player_input.write(movementflag);
    msg_player_input.finalize();


    int sent = send_all(client_socket, msg_player_input);
    if (sent > 0) {
        //std::cout << "message: ";
        //msg_player_input.print();
        //std::cout << std::endl;
    }
    if (sent < 0) {
        std::cout << "movementflag cannot be sent! " << std::endl;
        return -1;
    }

    return sent;
}


std::vector<playerState> Client::deserialize_playerstates(std::vector<Message>& msg_buffer, std::vector<playerState>& player_state_list) {

    for (auto& msg : msg_buffer) {

        if (msg.type() == messageType::playerState) {
            auto ps_buffer = msg.readps();

            auto it = std::find_if(player_state_list.begin(), player_state_list.end(), [&](const playerState& player_state) {
                return player_state.id == ps_buffer.id;
                });

            if (it != player_state_list.end()) {
                *it = playerState(ps_buffer.id, ps_buffer.playerpos.x, ps_buffer.playerpos.y);
            }
            else {
                player_state_list.push_back(playerState(ps_buffer.id, ps_buffer.playerpos.x, ps_buffer.playerpos.y));
            }
        }
    }

    return player_state_list;
}




