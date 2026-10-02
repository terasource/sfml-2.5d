#include "client.hpp"


ssize_t Client::send_all(int& socket, const Message& msg){
    ssize_t bytes_to_send = msg.data().size();

    const std::vector<std::uint8_t>& buffer = msg.data();

    ssize_t total_bytes_sent = 0;

    while(total_bytes_sent < bytes_to_send){

        ssize_t bytes_sent = send(socket, buffer.data() + total_bytes_sent, bytes_to_send - total_bytes_sent, 0);
        
        if(bytes_sent == -1){
            perror("sent");
            return -1;
        }

        total_bytes_sent += bytes_sent;
    }

    return total_bytes_sent;
}

//shrink this function into only to return std::vector not nested vector.
std::vector<std::vector<uint8_t>> Client::recv_all(int& scket, int& bytes_processed, int timeout){
    std::vector<std::vector<uint8_t>> frames;
    std::vector<uint8_t> frame;
    std::vector<uint8_t> temp_buffer(512);
    int total_received_bytes = 0;
    int bytes_expected_to_receive = 512;
    int bytes_to_processed = 0;
    int total_bytes_processed = 0;
    PacketReader pr(temp_buffer);

    
    while(true){
        pollfd pfd;
        pfd.fd = scket;
        pfd.events = POLLIN;
        int ret = poll(&pfd, 1, 0);

        if(ret < 0){
            perror("poll");
            break;
        }

        if(ret == 0)
            break;

        if(pfd.revents & POLLIN){
        int received = recv(scket, temp_buffer.data() + bytes_to_processed, 512 - bytes_to_processed, 0);
        
        //in order not to overwrite the 0.5 frame in 1.5 frame and etc.
        if(received > 0)
            bytes_to_processed += received;

      
            //what if i takes bytes in range 0-3. program will be looped forever.
            while(bytes_to_processed >= 4){
                    int frame_length = (pr.read_uint32_at(0) + 4 + 1);

                    if(bytes_to_processed < frame_length)
                        break;

                    if(bytes_to_processed >= frame_length){
                        frame.insert(frame.begin(), temp_buffer.begin(), temp_buffer.begin() + frame_length);
                        temp_buffer.erase(temp_buffer.begin(), temp_buffer.begin() + frame_length);
                        temp_buffer.resize(temp_buffer.size() + frame_length);
                        bytes_to_processed -= frame_length;
                        total_bytes_processed += frame_length;
                    }

                    if(frame.size() == frame_length){
                         frames.push_back(frame);  
                         frame.clear();
                    }
                                                     
                }

                if(received == 0){
                    //std::cout << "frame bitti mk gelmiyo dahaaaa " << std::endl;
                    close(scket);
                    break;
                }   

                if(received == -1){
                    perror("recv");
                    std::exit(1);
                }
            
            //std::cout << "received bytes = " << total_received_bytes << " remaining bytes = " << bytes_to_receive << std::endl;  
        }

    }
    bytes_processed = total_bytes_processed;
    return frames;
}

int Client::initialize_client(){
    addrinfo hints{};
    addrinfo* server_info;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    // skip the ai_protocol AI_PASSIVE here because this is client not a server accepts a connection.

    getaddrinfo("127.0.0.1", "1280", &hints, &server_info);

    int client_socket = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

    if(client_socket == -1)
        perror("socket");
        
    connect(client_socket, server_info->ai_addr, server_info->ai_addrlen);

    return client_socket;
}

void Client::close_client(int& client_socket){

    shutdown(client_socket, SHUT_WR);

    close(client_socket);   
}

void Client::client_handle_input(int& client_socket){
    inputHandler inputhandler;
    std::uint8_t movementFlag = inputhandler.collectInput();
    Message input(messageType::playerInput);
    input.write(movementFlag);
    input.finalize();
    
    int sent_all = send_all(client_socket, input);
    if(sent_all < 0)
        return;

    input.read();
}

playerState Client::deserialize_playerstate(std::vector<std::uint8_t>& buffer){

        //resolve this coordinate bugs.
        PacketReader pr(buffer);

        int msg_length = pr.read_uint32();
        int msg_type = pr.read_uint8();
        float pl_pos_x = pr.read_float();
        float pl_pos_y = pr.read_float();
 
        playerState pstate(1, pl_pos_x, pl_pos_y);
        std::cout << "playerstate x: " << pl_pos_x << " playerstate y: " << pl_pos_y << std::endl;
        return pstate; 
}

std::vector<std::uint8_t> Client:: read_playerstate_from_server(int& client_socket, int& bytes_received, int timeout){
   
    std::vector<std::vector<std::uint8_t>> vec = recv_all(client_socket, bytes_received, timeout);
    std::vector<std::uint8_t> result;
    if(vec.size() > 0)
        result.insert(result.begin(), vec.at(0).begin(), vec.at(0).end());
    else
        std::cout << "knk buffer bombos" << std::endl;

    return result;
}

ssize_t Client::send_movementflag(int& client_socket, std::uint8_t movementflag){
    Message msg_player_input(messageType::playerInput);
    msg_player_input.write(movementflag);
    msg_player_input.finalize();



    int sent = send_all(client_socket, msg_player_input);
    if(sent > 0){
        std::cout << "message: ";
        msg_player_input.read();
        std::cout << std::endl;
    }
    if(sent < 0){
        std::cout << "movementflag cannot be sent! " << std::endl;
        return -1;
    }

    return sent;
}