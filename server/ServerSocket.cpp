#include "ServerSocket.h"

ServerSocket::ServerSocket(int port) : serverSocket(-1), port(port) {}


void ServerSocket::init() {
    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Bind socket
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(serverSocket, 5) < 0) {
        perror("Error listening");
        exit(EXIT_FAILURE);
    }
}

int ServerSocket::acceptClient() {
    int clientSocket;
    struct sockaddr_in clientAddr;
    socklen_t clientLen = sizeof(clientAddr);

    // Accept incoming connection
    clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientLen);
    if (clientSocket < 0) {
        perror("Error accepting client");
        exit(EXIT_FAILURE);
    }

    return clientSocket;
}

void ServerSocket::closeSocket() {
    close(serverSocket);
}

std::string ServerSocket::receiveRequest(int clientSocket) {
    char buffer[1024] = {0};
    int valread = read(clientSocket, buffer, 1024);
    if (valread < 0) {
        perror("Error reading from client");
        exit(EXIT_FAILURE);
    }
    return std::string(buffer);
}

void ServerSocket::sendResponse(int clientSocket, const std::string& response) {
    send(clientSocket, response.c_str(), response.length(), 0);
}
