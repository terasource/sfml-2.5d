#include "server.hpp"

struct clientConnection {
    int skt_fd;
    int cl_id;
    std::vector<uint8_t> recv_buffer;
};

int main() {
    Server server;

    bool running = 1;
    int listening_socket = 0;
    int client_socket = server.initialize_server(listening_socket);
    int sent_res = 1;
    int bytes_processed = 0;
    int server_loop = 1;
    int next_client_id = 1;

    std::vector<std::vector<std::uint8_t>> buffer;
    playerState p1_state(1, 0, 0);

    std::vector<playerState> player_states;

    std::vector<pollfd> pollfds;

    pollfd listening_pfd;

    listening_pfd.events = POLLIN;
    listening_pfd.fd = listening_socket;
    pollfds.push_back(listening_pfd);

    std::vector<clientConnection> clients;

    while (running) {

        /*
            int ret = poll(pollfds.data(), pollfds.size(), 0);

        if (ret <= 0)
            continue;

        for (auto& pollfd : pollfds) {

            if (pollfd.events & POLLIN) {

                if (pollfd.fd == listening_socket) {
                    sockaddr_storage their_addr;
                    socklen_t their_size = sizeof(their_addr);
                    int new_fd = accept(listening_socket, reinterpret_cast<sockaddr*>(&their_addr), &their_size);

                    clientConnection cc{
                        .skt_fd = new_fd,
                        .cl_id = next_client_id++
                    };

                    clients.push_back(cc);
                }

                for (auto& client : clients) {

                    if (pollfd.fd == client.skt_fd) {
                        int bytes_processed = 0;

                        //this may cause double loop becasuse of the logic of recv_all. check it later.
        client.recv_buffer = server.recv_all(client.skt_fd, bytes_processed)[0];

    }

}

            }

        }
        */


        std::cout << "server loop: " << server_loop << std::endl;

        buffer = server.recv_all(client_socket, bytes_processed);
        if (bytes_processed > 0) {
            position& prev_char_pos = p1_state.get_player_position();
            std::cout << "receiving bytes successfull, received bytes: " << bytes_processed << " received movement flag: " << std::bitset<8>(buffer[0][5]) << std::endl;
            std::cout << "previous player pos x: " << prev_char_pos.x << " y: " << prev_char_pos.y << std::endl;
            auto new_playerState = server.calculate_playerState(buffer[0], prev_char_pos);

            //seperate getting the values for calculating playerstate and calculate process.
            std::cout << "calculated playerstate pos x: " << new_playerState.get_player_position().x << " y: " << new_playerState.get_player_position().y << std::endl;
            int sent_res = server.send_playerstate_to_client(client_socket, new_playerState);

            if (sent_res > 0) {
                std::cout << "sending playerstate is successfull, sent bytes: " << sent_res << std::endl;
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