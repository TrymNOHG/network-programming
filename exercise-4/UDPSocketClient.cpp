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

#define PORT "8080"

int main() {

    int server_socket_fd, addr_status;
    addrinfo server_address{0}, *server_info;
    socklen_t address_len = 0;

    server_address.ai_family = AF_UNSPEC; //Specifically using IPv6
    server_address.ai_socktype = SOCK_DGRAM;

    char host_name[256];
    gethostname(host_name, sizeof(host_name));

    addr_status = getaddrinfo(host_name, PORT, &server_address, &server_info);
    if(addr_status != 0) {
        perror("Getting the address info failed.");
        exit(EXIT_FAILURE);
    }

    while(server_info != nullptr){
        server_socket_fd = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol); // Creating the actual socket
        if(server_socket_fd < 0) {
            close(server_socket_fd);
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

    while(1) {
        InputData input{0};
        address_len = sizeof(server_address);

        std::cout << "Enter the equation you want checked.\n";
        std::cin >> input.num_1;
        std::cin >> input.op;
        std::cin >> input.num_2;

        while(sendto(server_socket_fd, &input, sizeof(input), 0, server_info->ai_addr, server_info->ai_addrlen) == 0) {
            perror("Send error");
        }

        double num;
        int bytes_recv = (int) recvfrom(server_socket_fd, &num, sizeof(double), MSG_WAITALL, server_info->ai_addr, &server_info->ai_addrlen);

        if(bytes_recv == 0) perror("Recovery error");
        std::cout << "=" << num << std::endl;
    }

    freeaddrinfo(server_info);
    close(server_socket_fd);
}