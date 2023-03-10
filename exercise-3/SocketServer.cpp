//
// Created by Trym Hamer Gudvangen on 2/15/23.
// g++ SocketServer.cpp -std=c++17 -o Server
//

#include <sys/socket.h>
#include <cstdio>
#include <unistd.h>
#include <iostream>
#include <netinet/in.h>
#include <vector>
#include <thread>
#include <cstring>

#include "SocketData.cpp"

#define PORT 1250
#define IP_VAL 0

double calculate(InputData inputData);

int main() {

    std::vector<std::thread> thread_pool;
    char msg_buffer[1024];

    int option = 1;
    int server_socket_fd = socket(AF_INET, SOCK_STREAM, IP_VAL); // Creating the actual socket
    if(server_socket_fd < 0) perror("Failed to create socket.");
    else std::cout << "Socket was successfully opened\n";

//    int socketOpt = setsockopt(serverSocketFD, SOL_SOCKET, SO_REUSEPORT | SO_REUSEADDR, &option, sizeof(option));
//    if(socketOpt < 0) perror("Socket address/port reuse failed");

    struct sockaddr_in address = {0};

    int address_len = sizeof(address);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; //Binding the socket to any address
    address.sin_port = htons(PORT); //The htons function takes a 16-bit host byte order and converts it to TCP/IP byte order

    //Attach socket to specified Port
    int bind_stat = bind(server_socket_fd, (const struct sockaddr*)&address, address_len);
    if (bind_stat < 0) perror("The socket failed to attach to the specified Port");
    else std::cout << "The socket has been attached to Port " << PORT << std::endl;

    if(listen(server_socket_fd, 10)) perror("Server socket connection error");
    bool exit = false;

    while(!exit) {
        int new_socket_fd = accept(server_socket_fd, (struct sockaddr*)&address, (socklen_t *)&address_len);
        if (new_socket_fd < 0) perror("New connection failed.");
        else {
            thread_pool.emplace_back([new_socket_fd, msg_buffer] {
                const char *return_msg = "You have successfully connected to the socket!\n";
                send(new_socket_fd, return_msg, strlen(return_msg), 0);

                InputData inputData = {0};
                long client_stat = recv(new_socket_fd, &inputData, sizeof(inputData), 0);
                while(client_stat > 0) {
                    double return_val = calculate(inputData);
                    send(new_socket_fd, &return_val, sizeof(double), 0);
                    inputData = {0};
                    client_stat = recv(new_socket_fd, &inputData, sizeof(inputData), 0);
                }
                close(new_socket_fd);
            });
        }
        char ans;
        std::cout << "Do you want to keep the server running? (Y/N)";
        std::cin >> ans;
        if(ans == 'N') exit = true;
    }

    shutdown(server_socket_fd, SHUT_RDWR);

    for (auto& thread : thread_pool) {
        thread.join();
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


