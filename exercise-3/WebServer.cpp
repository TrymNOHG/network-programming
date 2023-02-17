//
// Created by Trym Hamer Gudvangen on 2/16/23.
//

#include <sys/socket.h>
#include <cstdio>
#include <unistd.h>
#include <iostream>
#include <netinet/in.h>
#include <vector>
#include <thread>
#include <cstring>

#define PORT 8080
#define IP_VAL 0

void add_string_to_buffer(char msg_buffer[1024 * 16], int &index, char *string);

int main() {


    std::vector<std::thread> thread_pool;

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
        int new_socket_fd = accept(server_socket_fd, (struct sockaddr*)&address, (socklen_t *)&address_len); //Here multiple threads can be made.
        if (new_socket_fd < 0) perror("New connection failed.");
        else {
            thread_pool.emplace_back([new_socket_fd] {
                char msg_buffer[1024 * 16];
                char recv_msg[1024];
                int current_index = 0;

                if (recv(new_socket_fd, &recv_msg, sizeof(recv_msg), 0) < 1) perror("Recv fail");
                const char *server_msg = "HTTP/1.0 200 OK\n"
                                         "Content-Type: text/html; charset=utf-8\n"
                                         "\n<HTML><BODY>\n"
                                         "<H1> Hilsen. Du har koblet deg opp til min enkle web-tjener </h1>\n"
                                         "Header fra klient er:<br>";


                add_string_to_buffer(msg_buffer, current_index, (char *) server_msg);



                add_string_to_buffer(msg_buffer, current_index, (char *) "<UL>\n");
                std::string list_point;
                for (auto &charac : recv_msg) {
                    if(charac == '\n') {
                        add_string_to_buffer(msg_buffer, current_index, (char *) "<LI>");
                        add_string_to_buffer(msg_buffer, current_index, (char *) list_point.c_str());
                        add_string_to_buffer(msg_buffer, current_index, (char *) "</LI>");
                        list_point = "";
                    } else {
                        list_point += charac;
                    }
                }
                add_string_to_buffer(msg_buffer, current_index, (char *) "</BODY></HTML>");

                send(new_socket_fd, msg_buffer, strlen(msg_buffer), 0);
                std::this_thread::sleep_for(std::chrono::seconds(5));
                close(new_socket_fd);
            });
        }
        char ans;
        std::cout << "Do you want to keep the server running? (Y/N)\n";
        std::cin >> ans;
        if(ans == 'N') exit = true;
    }

    shutdown(server_socket_fd, SHUT_RDWR);

    for (auto& thread : thread_pool) {
        thread.join();
    }

    return 0;
}

void add_string_to_buffer(char msg_buffer[1024 * 16], int &index, char *string) {
    for (int i = 0; i < strlen(string); i++) {
        msg_buffer[index++] = string[i];
    }
}
