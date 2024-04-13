#ifndef SERVER_SOCKET_H
#define SERVER_SOCKET_H

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

class ServerSocket {
private:
    int serverSocket;
    int port;
    struct sockaddr_in serverAddr;

public:
    ServerSocket(int port);
    void init();
    int acceptClient();
    void closeSocket();
    std::string receiveRequest(int clientSocket);
    void sendResponse(int clientSocket, const std::string& response);
};

#endif // SERVER_SOCKET_H
