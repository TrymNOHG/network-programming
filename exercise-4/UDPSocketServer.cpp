//
// Created by Trym Hamer Gudvangen on 2/20/23.
// g++ UDPSocketServer.cpp -std=c++17 -o Server
//

#include <sys/socket.h>
#include <cstdio>
#include <unistd.h>
#include <iostream>
#include <netinet/in.h>
#include <vector>
#include <thread>
#include <cstring>
#include <netdb.h>

#include "SocketData.cpp"

#define PORT "1250"
#define IP_VAL 0

double calculate(InputData inputData);
void *config_in_addr(sockaddr &sa);

int main() {

    int server_socket_fd, addr_status;
    std::vector<std::thread> thread_pool;
    addrinfo addr_hints{0}, *server_info;
    socklen_t address_len;
    sockaddr_storage client_addr{};

    addr_hints.ai_family = AF_UNSPEC;
    addr_hints.ai_socktype = SOCK_DGRAM; //Binding the socket to any address
    addr_hints.ai_protocol = IPPROTO_UDP;
    addr_hints.ai_flags = AI_PASSIVE;

    addr_status = getaddrinfo(nullptr, PORT, &addr_hints, &server_info);
    if(addr_status != 0) {
        perror("Getting the address info failed.");
        exit(EXIT_FAILURE);
    }

    while(server_info->ai_next != nullptr){
        server_socket_fd = socket(server_info->ai_family, SOCK_DGRAM, server_info->ai_protocol); // Creating the actual socket
        if(server_socket_fd < 0) {
            perror("Failed to create socket.");
            server_info = server_info->ai_next;
            continue;
        }

        int bind_stat = bind(server_socket_fd, server_info->ai_addr, server_info->ai_addrlen);
        if (bind_stat < 0) {
            perror("The socket failed to attach to the specified Port");
            server_info = server_info->ai_next;
            continue;
        }
        else std::cout << "The socket has been attached to Port " << PORT << std::endl;

        break;
    }

    //TODO: doesn't reach here

    if (server_info == nullptr) {
        std::cout << "There was no valid socket opened.";
        exit(EXIT_FAILURE);
    }



    InputData inputData{0};
    address_len = sizeof(client_addr);
    while(recvfrom(server_socket_fd, &inputData, sizeof(inputData), 0, &(struct sockaddr &)client_addr, &address_len) == -1) {
        perror("Recovering message failed");
    }

    double return_val = calculate(inputData);
    int bytes_sent = (int) sendto(server_socket_fd,
                            &return_val, sizeof(double),
                            0, &(struct sockaddr &)client_addr, address_len);
    if(bytes_sent == -1) perror("Send error");
    inputData = {0};



//    bool exit = false;
//    while(!exit) {
//            thread_pool.emplace_back([] {
//                const char *return_msg = "You have successfully connected to the socket!\n";
//                sendto(new_socket_fd, return_msg, strlen(return_msg), 0);
//
//                InputData inputData = {0};
//                long client_stat = recvfrom(new_socket_fd, &inputData, sizeof(inputData), 0);
//                while(client_stat > 0) {
//                    double return_val = calculate(inputData);
//                    sendto(new_socket_fd, &return_val, sizeof(double), 0);
//                    inputData = {0};
//                    client_stat = recvfrom(new_socket_fd, &inputData, sizeof(inputData), 0);
//                }
//                close(new_socket_fd);
//            });
//        char ans;
//        std::cout << "Do you want to keep the server running? (Y/N)";
//        std::cin >> ans;
//        if(ans == 'N') exit = true;
//    }
    freeaddrinfo(server_info);

    shutdown(server_socket_fd, SHUT_RDWR);
//
//    for (auto& thread : thread_pool) {
//        thread.join();
//    }


}

double calculate(InputData inputData) {

    switch(inputData.op) {
        case '+':
            return inputData.num_1 + inputData.num_2;
        case '-':
            return inputData.num_1 - inputData.num_2;
        case '*':
            return inputData.num_1 * inputData.num_2;
        case '/':
            return inputData.num_1 / inputData.num_2;
    }

    return -1;
}

void *config_in_addr(sockaddr &sa) {
    if (sa.sa_family == AF_INET) return &(((sockaddr_in &)sa).sin_addr);
    else return &(((sockaddr_in6 &)sa).sin6_addr);
}