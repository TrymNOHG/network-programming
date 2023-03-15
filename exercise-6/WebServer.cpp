/* Example of opening handshake from Server:

        HTTP/1.1 101 Switching Protocols
        Upgrade: websocket
        Connection: Upgrade
        Sec-WebSocket-Accept: s3pPLMBiTxaQ9kYGzzhZRbK+xOo=
        Sec-WebSocket-Protocol: chat

 */

#include <iostream>
#include <openssl/sha.h>
#include <sys/socket.h>
#include <cstdio>
#include <netinet/in.h>
#include <thread>
#include <cstring>
#include <unistd.h>
#include <vector>


#define GUID "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"
#define WEBSOCKET_PORT 3001

bool c_pointer_contains(char *string, char *sub_string);
char *concat_char_pointers(char *first, char *second);
std::vector<char*> split_char_pointer(char *string, char delimiter);
char *base64_encoding(unsigned char *string);
char *opening_handshake(char* client_handshake);
char *closing_handshake();

int main() {


//    struct sockaddr_in address = {0};
//    int address_len = sizeof(address);
//
//    int server_socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//    if(server_socket_fd == -1) perror("Failed to initialize socket.");
//
//    address.sin_family = AF_INET;
//    address.sin_addr.s_addr = INADDR_ANY;
//    address.sin_port = htons(HTTP_PORT);
//
//    int bind_stat = bind(server_socket_fd, (const struct sockaddr*)&address, address_len);
//    if (bind_stat < 0) perror("The socket failed to attach to the specified Port");
//    else std::cout << "The socket has been attached to Port " << HTTP_PORT << std::endl;
//
//    if(listen(server_socket_fd, 10)) perror("Server socket connection error");
//
//
//    int new_socket_fd = accept(server_socket_fd, (struct sockaddr*)&address, (socklen_t *)&address_len);
//    if (new_socket_fd < 0) perror("New connection failed.");
//
////
//
//    const char *server_msg = "HTTP/1.0 200 OK\n"
//                             "Content-Type: text/html; charset=utf-8\n"
//                             "<!DOCTYPE html>\n"
//                                "\n<html>\n"
//                                    "<head>\n"
//                                        "<meta charset=\"UTF-8\" />\n"
//                                    "</head>\n"
//                                    "<body>\n"
//                                        "WebSocket test page\n"
//                                        "<script>\n"
////                                        "let ws = new WebSocket(\'ws://localhost:3001\');\n" //TODO: change to create a new websocket
////                                        "ws.onmessage = event => alert(\'Message from server: \' + event.data);\n"
////                                        "ws.onopen = () => ws.send(\'hello\');\n"
//                                        "</script>\n"
//                                    "</body>\n"
//                                "</html>";
//
//    if(send(new_socket_fd, server_msg, strlen(server_msg), 0) < 1) perror("Unsuccessfully sent");
//    shutdown(server_socket_fd, SHUT_WR);
//
//    char recv_msg[1024];
//    if (recv(new_socket_fd, &recv_msg, sizeof(recv_msg), 0) < 1) perror("Recv fail");
//
////    std::cout << recv_msg;
//
//    close(new_socket_fd);

    struct sockaddr_in address = {0};
    int address_len = sizeof(address);

    int server_socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(server_socket_fd == -1) perror("Failed to initialize socket.");
    else std::cout << "Socket successfully created.\n";

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(WEBSOCKET_PORT);

    int bind_stat = bind(server_socket_fd, (const struct sockaddr*)&address, address_len);
    if (bind_stat < 0) perror("The socket failed to attach to the specified Port");
    else std::cout << "The socket has been attached to Port " << WEBSOCKET_PORT << std::endl;

    if(listen(server_socket_fd, 10)) perror("Server socket connection error");


    int new_socket_fd = accept(server_socket_fd, (struct sockaddr*)&address, (socklen_t *)&address_len);
    if (new_socket_fd < 0) perror("New connection failed.");
    else std::cout << "Socket connected\n";

    char client_handshake[1024];
    if (recv(new_socket_fd, &client_handshake, sizeof(client_handshake), 0) < 1) perror("Recv fail");

    std::cout << client_handshake;

    char *server_handshake = opening_handshake(client_handshake);
    send(new_socket_fd, server_handshake, strlen(server_handshake), 0);


    return 0;
}

/**
 * This method checks if a char pointer contains another char pointer.
 * @param string        String to be check, represented as a char pointer.
 * @param sub_string    Sub-string, represented as a char pointer.
 * @return              True if sub_string exists in string, else False
 */
bool c_pointer_contains(char *string, char *sub_string) {
    for(char *letter = string; letter < string + strlen(string); letter++) {
        if(*letter == *sub_string) {
            char *sub_letter = sub_string;
            while(*++letter == *++sub_letter
            && letter < string + strlen(string)
            && sub_letter < sub_string + strlen(sub_string));
            if(sub_letter == sub_string + strlen(sub_string)) return true;
        }
    }
    return false;
}

char *concat_char_pointers(char *first, char *second) {
    unsigned int size_of_buffer = strlen(first) + strlen(second);
    char *buffer = (char*) malloc(sizeof(char) * size_of_buffer);
    for(int i = 0; i < strlen(first); i++) *(buffer + i) = *(first + i);
    for(unsigned int i = strlen(first); i < size_of_buffer; i++) *(buffer + i) = *(second++);
    return buffer;
}

std::vector<char*> split_char_pointer(char *string, char delimiter) {
    std::vector<char*> elements;
    for(unsigned int i = 0; i < strlen(string); i++) {
        if(*(string + i) == '\n') {
            char *buffer = (char *)malloc(sizeof(char) * ++i);
            strlcpy(buffer, string, i);
            elements.emplace_back(buffer);
            string += i;
            i = 0;
        }
    }

    char *buffer = (char *)malloc(sizeof(char) * strlen(string) + 1);
    strlcpy(buffer, string, strlen(string) + 1);
    elements.emplace_back(buffer);

    return elements;
}


char *base64_encoding(unsigned char *string) {

    char base64_ASCII[64] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                             'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                             'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                             'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                             'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                             'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                             'w', 'x', 'y', 'z', '0', '1', '2', '3',
                             '4', '5', '6', '7', '8', '9', '+', '/'};

    char *encoded_msg = (char*)malloc(sizeof(char) * strlen((const char*)string) * (4/3));
    int index = 0;
    for(unsigned char *triad = string; triad < string + strlen((const char*)string); triad+=3) {
        *(encoded_msg + index++) = base64_ASCII[((int)*triad >> 2)];
        *(encoded_msg + index++) = base64_ASCII[((((int)*triad & 3) << 4) | ((int)*(triad+1) >> 4))];
        *(encoded_msg + index++) = base64_ASCII[((((int)*(triad+1) & 15) << 2) | ((int)*(triad + 2) >> 6))];
        *(encoded_msg + index++) = base64_ASCII[((int)*(triad+2) & 63)];
    }

    unsigned int padding = 3 - strlen((const char*)string) % 3;
    for(unsigned int i = padding; i > 0; i--) *(encoded_msg + index - i) = '=';

    return encoded_msg;
}

char* opening_handshake(char* client_handshake) {
    std::vector<char*> elements_of_handshake = split_char_pointer(client_handshake, '\n');

    char *new_msg = elements_of_handshake[0];
    //TODO: look at the different built-in char pointer methods. Maybe I could properly isolate the elements in the vector.

    int HTTP_status = 101;

    //TODO: include a reactive HTTP status in the HTTP_status line; Could do this with sprintf
    char *HTTP_status_line;

    char* handshake = (char*)"HTTP/1.1 101 Switching Protocols\n"
                      "Upgrade: websocket\n"
                      "Connection: Upgrade\n";


    char *sec_webSocket_key;

    for(auto& element : elements_of_handshake) {
        if(c_pointer_contains(element, (char*)"Sec-WebSocket-Key:")) {
            while(*element++ != ':' || *element++ != ' ');
            sec_webSocket_key = (char*) malloc(sizeof(char) * strlen(element));
            strlcpy(sec_webSocket_key, element, strlen(element));
            break;
        }
    }

    auto response_val = (const unsigned char*) concat_char_pointers(sec_webSocket_key, (char*)GUID);
    auto *message_digest = (unsigned char*)malloc(sizeof(char) * 20);
    SHA1(response_val, strlen((char *)response_val),message_digest);

    char *base64_response = base64_encoding(message_digest);

    free(sec_webSocket_key);
    free((char *)response_val);
    free(message_digest);


    char *sec_webSocket_accept = concat_char_pointers((char*)"Sec-WebSocket-Accept: ", base64_response);

    char *full_handshake = (char*) malloc(sizeof(char) * 1024);
    snprintf(full_handshake, 1024 ,"%s%s\r\n\r\n", handshake, sec_webSocket_accept);

    return full_handshake;
}

char *closing_handshake() {
    return nullptr;
}

/*
 * Unit used to send information: messages, which is composed of one or more frames.
 * A frame has an associated type. Each frame belonging to the same message
 * contains the same type of data.
 *
 * This protocol defines 6 frame types.
 *
 * TODO: How should I handle the different frame types? How should I compose a message? Common class for message?
 *
 */