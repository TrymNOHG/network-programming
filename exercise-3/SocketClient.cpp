//
// Created by Trym Hamer Gudvangen on 2/15/23.
//

#include "SocketClient.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <cstdio>
#include <unistd.h>
#include <iostream>
#include <thread>
#include <cstring>
#include <arpa/inet.h>

#define PORT 1250
#define IP_VAL 0

int main() {

    struct sockaddr_in server_address;
    char msg_buffer[1024];

    int option = 1;
    int socket_fd = socket(AF_INET, SOCK_STREAM, IP_VAL); // Creating the actual socket
    if(socket_fd < 0) perror("Failed to initialize socket.");
    else std::cout << "Socket was successfully opened\n";

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT); //The htons function takes a 16-bit host byte order and converts it to TCP/IP byte order

    const char *ipAddress = "127.0.0.1";
    int network_addr_stat = inet_pton(server_address.sin_family, ipAddress, &server_address.sin_addr);
    if(network_addr_stat < 0) perror("IP Address is invalid.");

    int client_fd = connect(socket_fd, (const struct sockaddr*)&server_address, sizeof(server_address));
    if (client_fd < 0) perror("Attempt to connect to server was unsuccessful. Try again.");

//    send(socketFD, "Hello", strlen("Hello"), 0);
//    send(socketFD, "Hello", strlen("Hello"), 0);
    recv(socket_fd, msg_buffer, 1024, 0);
    std::cout << msg_buffer;

    std::cout << "To exit the socket, Type Quit\n";
    while(true) {
        std::string send_message;
        std::cout << "Enter the equation you want checked: number+-number\n";

        std::cin >> send_message;
        if (send_message == "Quit") break;
        send(socket_fd, send_message.c_str(), strlen(send_message.c_str()), 0);

        recv(socket_fd, msg_buffer, 1024, 0);
        std::cout << msg_buffer[0] - '0';
    }


    close(client_fd);

}