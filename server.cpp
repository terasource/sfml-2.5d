#include "server.hpp"


std::vector<std::vector<uint8_t>> Server::recv_all(int scket, int& bytes_processed){
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
        // 2ms timeout. otherwise cpu is frying fkn reached 101celcius
        int ret = poll(&pfd, 1, 2);

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

ssize_t Server::send_all(int socket, const Message& msg){
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

void Server::print_frames(std::vector<std::vector<std::uint8_t>> frames){
    for(size_t i = 0; i < frames.size(); i++){
            std::cout << "reading frame #" << i << " was successfull." << std::endl;
        for(size_t j = 0; j < frames[i].size(); j++){
                std::cout << std::hex << static_cast<int>(frames[i][j]);
                    if(i < frames[i].size() - 1)
                        std::cout << ", ";
            }

            std::cout << std::endl;
        }  
}

position Server::calculate_char_pos(PacketReader& pr, position& pos){
    std::uint8_t movementflag = pr.read_uint8_at(5);
    //std::cout << "readed movementflag at calculate_char_pos: " << std::bitset<8>(movementflag) << std::endl;

    int speed = 10;
    for(int i = 0; i < 5; i++){
         // 00011111 shiftwasd 
        std::uint8_t bit_i = movementflag & (1 << i);
        std::cout << "movementflag in for: " << std::bitset<8>(movementflag) << std::endl;
        std::cout << "bit_i = " << std::bitset<8>(bit_i) << std::endl;
        std::cout << "i = " << i << std::endl;
        speed = 10;
        //fkn compiler says == has high precedence than & okay i already want to do == first fkn keeps giving warning
        if((i == 4) << i & bit_i)
            speed = 20;
        if((i == 0) << i & bit_i){
            //std::cout << " d is working " << std::endl;
            pos.x += speed;
        }
        if((i == 1) << i & bit_i){
            //std::cout << "s is working " << std::endl;
            pos.y += speed;
            //std::cout << "pos : " << pos.y << std::endl;
        }
        if((i == 2) << i & bit_i)
            pos.x -= speed;
        if((i == 3) << i & bit_i)
            pos.y -= speed;      
    }
    
    // ahahah what a fking bug was mate. i was checking the indexes to determine what to do for each bit
    // and check if the bit_i was 1 so the flag was sent AND i equal to index in order to do correct calculation
    // but the ampersand actually compares all the bits of lvalue and right value.
    // i mean when i = 2 and if i check i == 2 this returns 1 in integer terms its only 1 but in bits
    // it is 00000001 but the issue occurs here. because i spread the bits on the flag and each bit represents
    // different state when i get bit_i i get all the std::uint8_t object itself i mean if the flag is 00000010
    // this mean s is pressed and sent from the client to server and i need to calculate the new pos of y.
    // but in the if block i compare them like (i == 1)(to decide what to do) & bit_i(to check if bit is 1 or not)
    // and here is the issue. 1 = 00000001 and bit_i = 00000010 if i compare them with and(&) this returns 0.
    // because bit_i = 2 in integer terms not 1. so 1 and 2 is not true.
    // so what did i do was i shifted the i == index state to left by i times in order to get the correct mask of
    // bit_i is holding. with this they have become equal and i finally check them correctly.

    std::cout << "calculated pos x: " << pos.x << " y: " << pos.y << std::endl;
    return pos;
}

int Server::initialize_server(int& listening_socket){
    std::cout << "calisiyom lan ben " << std::endl;
    //hints for the use of telling what kind of connection we want to build
    addrinfo hints{};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    // a pointer points to a addrinfo struct object to give getaddrinfo() function to fill it with created object based on the informations that we give with hint{} struct.
    addrinfo* server_info{}; 
    
    // getting informations to create a socket.
    int res = getaddrinfo(nullptr, "1280", &hints, &server_info);
    if(res == -1)
        perror("getaddrinfo");
    int sckt = 0;
    if(res == 0)
    // creating a socket which is a kernel communication object to manage communications at endpoint,
    // and we create it with the hint informations which we gave them above to determine what kind of connection we want.
    sckt = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);
    if(sckt == -1)
        perror("socket");


    int reuse = 1;
    if(setsockopt(sckt, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0){
        perror("setsockopt(SO_REUSEADDR) failed");
        close(sckt);
    }
    
    // binding endpoint with the kernel
    int bnd = bind(sckt, server_info->ai_addr, server_info->ai_addrlen);

    if(bnd == -1)
        perror("bind");

    int lsn = listen(sckt, 5);

    if (lsn == -1)
        perror("listen");
    

    // an enough storage for every family of incoming connection ipv4 or ipv6 addresses.
    sockaddr_storage their_addr;

    // size of the incoming address in order to tell kernel to you can only write a dedicated size of memory. we allocated it for you and you can use it.
    // this does not have to be equal to the actual size of the incoming package metadata
    // this is maximum.

    socklen_t their_size = sizeof(their_addr);
    // creating a new file descriptor to use on new connection with send() and receive() functions, listening socket file descriptor will be stay same in order to accept new connections in queue if there.
    // casting sockaddr* to sockaddr_storage because accept function is getting sockaddr* type in order to be generic where it can represente both ipv4 and ipv6 connections.
    // this is not a converting sockaddr_storage to sockaddr. this means we are pointing the sockaddr_storage type memory with sockaddr type. summition the same memory is pointed by 2 different types.
    int new_fd = accept(sckt, reinterpret_cast<sockaddr*>(&their_addr), &their_size); 
    if(new_fd == -1)
        perror("accept");


    listening_socket = sckt;
    return new_fd;    
}

void Server::close_sockets(int listening_socket, int connected_socket){
    close(listening_socket);
    close(connected_socket);
}

playerState Server::calculate_playerState(const std::vector<std::uint8_t>& buffer, position& prev_char_pos){
        
    PacketReader pr(buffer);
    playerState pstate;
    
    auto new_c_pos = calculate_char_pos(pr, prev_char_pos);
    std::cout << "character pos x: " << new_c_pos.x << " y: " << new_c_pos.y << std::endl;
    pstate.set_player_position({new_c_pos.x, new_c_pos.y});

    return pstate;
}

int Server::send_playerstate_to_client(const unsigned int& client_socket, playerState pstate){
    Message pstate_msg(messageType::playerState);
    
    //pstate_msg.write(pstate.id);
    pstate_msg.write(pstate.playerpos.x);
    pstate_msg.write(pstate.playerpos.y);

    pstate_msg.finalize();

    //int res = send(client_socket, pstate_msg.data().data(), pstate_msg.data().size(), 0);
    int res = send_all(client_socket, pstate_msg);


    return res;
}

