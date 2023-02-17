//
// Created by Trym Hamer Gudvangen on 2/15/23.
//

#include <netinet/in.h>
#include <sys/socket.h>
#include <cstdio>
#include <unistd.h>
#include <iostream>
#include <thread>
#include <cstring>
#include <arpa/inet.h>

#include "SocketData.cpp"

#define PORT 1250
#define IP_VAL 0

int main() {

    struct sockaddr_in server_address = {0};
    char msg_buffer[1024];

    int option = 1;
    int socket_fd = socket(AF_INET, SOCK_STREAM, IP_VAL); // Creating the actual socket
    if(socket_fd < 0) perror("Failed to initialize socket.");
    else std::cout << "Socket was successfully opened\n";

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT); //The htons function takes a 16-bit host byte order and converts it to TCP/IP byte order

    const char *ipAddress = "127.0.0.1"; //Standard Localhost IP
    int network_addr_stat = inet_pton(server_address.sin_family, ipAddress, &server_address.sin_addr);
    if(network_addr_stat < 0) perror("IP Address is invalid.");

    int client_fd = connect(socket_fd, (const struct sockaddr*)&server_address, sizeof(server_address));
    if (client_fd < 0) perror("Attempt to connect to server was unsuccessful. Try again.");

    recv(socket_fd, msg_buffer, 1024, 0);
    std::cout << msg_buffer;

    InputData input = {0};
    std::cout << "To exit the socket, Type Quit\n";
    while(true) {
        std::cout << "Enter the equation you want checked.\n";
        std::cin >> input.num_1;
        std::cin >> input.op;
        std::cin >> input.num_2;


        send(socket_fd, &input, sizeof(input), 0);
        double num;
        recv(socket_fd, &num, sizeof(double), 0);
        std::cout << "=" << num << std::endl;

        std::string send_message;
        std::cout << "Type \'Yes\' if you would like to disconnect the socket. Else \'No\'\n";
        std::cin >> send_message;
        if (send_message == "Yes") break;
    }

    close(client_fd);
}