#include "server.hpp"

struct clientConnection {
    int skt_fd;
    int cl_id;
};

int main() {
    Server server;

    bool running = 1;
    int listening_socket = server.initialize_server();
    int sent_res = 1;
    int bytes_processed = 0;
    int server_loop = 1;
    int next_client_id = 1;

    std::vector<std::vector<std::uint8_t>> buffer;

    std::vector<playerState> player_states;

    std::vector<pollfd> pollfds;

    pollfd listening_pfd;

    listening_pfd.events = POLLIN;
    listening_pfd.fd = listening_socket;
    pollfds.push_back(listening_pfd);

    std::vector<clientConnection> clients;

    while (running) {

        int ret = poll(pollfds.data(), pollfds.size(), -1);

        if (ret < 0)
            continue;

        //new client making a connection
        if (pollfds[0].revents & POLLIN) {
            sockaddr_storage their_addr;
            socklen_t their_size = sizeof(their_addr);
            int new_fd = accept(listening_socket, reinterpret_cast<sockaddr*>(&their_addr), &their_size);

            clientConnection cc{
                .skt_fd = new_fd,
                .cl_id = next_client_id++
            };

            int x = rand() % 100;
            int y = rand() % 100;

            playerState t_pstate({ cc.cl_id, x,y });
            player_states.push_back(t_pstate);
            clients.push_back(cc);

            pollfd cl_pfd;
            cl_pfd.events = POLLIN;
            cl_pfd.fd = new_fd;
            pollfds.push_back(cl_pfd);


            //sending snapshots to new connection client.
            server.send_playeridtoclient(new_fd, cc.cl_id);
            server.send_playerstatevector_to_client(new_fd, player_states);

            // sending new client's snapshots to existing clients.
            for (size_t i = 0; i < clients.size() - 1; i++) {
                server.send_playeridtoclient(clients[i].skt_fd, cc.cl_id);
                server.send_playerstate_to_client(clients[i].skt_fd, t_pstate);
            }

            std::cout << "client #" << cc.cl_id << " has joined the game!\n";
        }
        else {
            //this client handling is now in order by client ids but im not sure about the implementation

            for (auto& client : clients) {
                int bytes_processed = 0;
                int bytes_processed2 = 0;

                std::vector<std::vector<uint8_t>> client_recv_buffer = server.recv_all(client.skt_fd, bytes_processed);
                std::vector<Message> playerinput_flag_list = server.crbs_to_msg(client_recv_buffer);

                std::cout << "input from fd=" << client.skt_fd
                    << " player_id=" << client.cl_id
                    << '\n';


                auto it = std::find_if(player_states.begin(), player_states.end(), [&](const playerState& player_state) {
                    return player_state.id == client.cl_id;
                    });

                if (it != player_states.end()) {
                    position& prev_char_pos = it->get_player_position();
                    std::cout << "player id " << client.cl_id << "player pos x: " << prev_char_pos.x << " y: " << prev_char_pos.y << std::endl;
                    //std::cout << "receiving bytes successfull, received bytes: " << bytes_processed << " received movement flag: " << std::bitset<8>(playerinput_flag_list.at(p_state.id).data().at(5)) << std::endl;

                    //for all inputs
                    for (size_t i = 0; i < playerinput_flag_list.size(); i++) {
                        server.handle_playerstate(playerinput_flag_list[i], *it);
                    }


                    //seperate getting the values for calculating playerstate and calculate process.
                    //std::cout << "calculated playerstate pos x:"  << player_states[order].get_player_position().x << " y: " << player_states[order].get_player_position().y << std::endl;

                    int sent_res = server.send_playerstatevector_to_client(client.skt_fd, player_states);

                    if (sent_res > 0)
                        std::cout << "sending playerstate is successfull, sent bytes: " << sent_res << std::endl;
                }
                else {
                    //assert mk
                }
            }
        }

        server_loop++;
    }

    if (sent_res < 0) {
        std::cout << "unknown error occured on server side!" << std::endl;
        return -1;
    }

    std::cin.get();
    return 0;
}