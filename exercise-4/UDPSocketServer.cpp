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
#include <netdb.h>

#include "SocketData.cpp"

#define PORT "8080"

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

    while(server_info != nullptr){
        server_socket_fd = socket(server_info->ai_family, SOCK_DGRAM, server_info->ai_protocol); // Creating the actual socket
        if(server_socket_fd < 0) {
            perror("Failed to create socket.");
            server_info = server_info->ai_next;
            continue;
        }

        int bind_stat = bind(server_socket_fd, server_info->ai_addr, server_info->ai_addrlen);
        if (bind_stat < 0) {
            close(server_socket_fd);
            perror("The socket failed to attach to the specified Port");
            server_info = server_info->ai_next;
            continue;
        }
        else std::cout << "The socket has been attached to Port " << PORT << std::endl;


        break;
    }

    if (server_info == nullptr) {
        std::cout << "There was no valid socket opened.";
        exit(EXIT_FAILURE);
    }

    InputData inputData{0};
    address_len = sizeof(client_addr);

    while(1) {
        while(recvfrom(server_socket_fd, &inputData, sizeof(inputData), MSG_WAITALL, &(struct sockaddr &)client_addr, &address_len) == -1) {
            perror("Recovering message failed");
        }


        double return_val = calculate(inputData);
        int bytes_sent = (int) sendto(server_socket_fd,
                                      &return_val, sizeof(double),
                                      0, &(struct sockaddr &)client_addr, address_len);
        if(bytes_sent == -1) perror("Send error");
        std::cout << "Answer sent!" << std::endl;
        inputData = {0};
    }

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