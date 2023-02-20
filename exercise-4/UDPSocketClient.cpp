//
// Created by Trym Hamer Gudvangen on 2/20/23.
//

#include <sys/socket.h>
#include <cstdio>
#include <unistd.h>
#include <iostream>
#include <thread>
#include <netdb.h>

#include "SocketData.cpp"

#define PORT "1250"
#define IP_VAL 0

int main() {

    int server_socket_fd, addr_status;
    addrinfo server_address{0}, *server_info;
    socklen_t address_len = 0;

    server_address.ai_family = AF_UNSPEC;
    server_address.ai_socktype = SOCK_DGRAM;

    addr_status = getaddrinfo(nullptr, PORT, &server_address, &server_info);
    if(addr_status != 0) {
        perror("Getting the address info failed.");
        exit(EXIT_FAILURE);
    }


    int socket_fd = socket(PF_INET, SOCK_DGRAM, IP_VAL); // Creating the actual socket
    if(socket_fd < 0) perror("Failed to initialize socket.");
    else std::cout << "Socket was successfully opened\n";

    while(server_info->ai_next != nullptr){
        server_socket_fd = socket(server_info->ai_family, SOCK_DGRAM, server_info->ai_protocol); // Creating the actual socket
        if(server_socket_fd < 0) {
            perror("Failed to create socket.");
            server_info = server_info->ai_next;
            continue;
        }

        break;
    }

    if (server_info == nullptr) {
        std::cout << "There was no valid socket opened.";
        exit(EXIT_FAILURE);
    }

    InputData input = {0};
    address_len = sizeof(server_address);

    std::cout << "Enter the equation you want checked.\n";
    std::cin >> input.num_1;
    std::cin >> input.op;
    std::cin >> input.num_2;

    while(sendto(socket_fd, &input, sizeof(input), 0, &(struct sockaddr &)server_address, address_len) == 0) {
        perror("Send error");
    }

    double num;
    //TODO: fix the method below.
    int bytes_recv = (int) recvfrom(socket_fd, &num, sizeof(double), 0, &(struct sockaddr &)server_address, &address_len);
    if(bytes_recv == 0) perror("Recovery error");
    std::cout << "=" << num << std::endl;


    freeaddrinfo(server_info);


//    while(true) {
//        std::cout << "Enter the equation you want checked.\n";
//        std::cin >> input.num_1;
//        std::cin >> input.op;
//        std::cin >> input.num_2;
//
//
//        send(socket_fd, &input, sizeof(input), 0);
//        double num;
//        recv(socket_fd, &num, sizeof(double), 0);
//        std::cout << "=" << num << std::endl;
//
//        std::string send_message;
//        std::cout << "Type \'Yes\' if you would like to disconnect the socket. Else \'No\'\n";
//        std::cin >> send_message;
//        if (send_message == "Yes") break;
//    }

    close(server_socket_fd);
}