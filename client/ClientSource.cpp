#include "ClientHeader.h"
#include "JsonHandler.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include <cstring>

#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT 8080

Client::Client(const std::string& username) : username(username) {
    // Create socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        perror("Error creating socket");
        exit(1);
    }

    // Configure server address
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);

    // Connect to server
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Error connecting to server");
        exit(1);
    }

    // Send username to server
    if (send(clientSocket, username.c_str(), username.length(), 0) < 0) {
        perror("Error writing to server");
        exit(1);
    }

    std::cout << "Connected to server." << std::endl;
}

void Client::sendMessage(const std::string& requestType) {
    // Send message feature implementation
    std::string receiver, message;
    std::cout << "Enter receiver username: ";
    std::getline(std::cin>>std::ws, receiver);
    std::cout << "Enter message: ";
    std::getline(std::cin, message);

    // Send requestType, sender, receiver, and message to server by JSON
    std::string jsonData = JsonHandler::serializeToJson(requestType, username, receiver, message);
    if (write(clientSocket, jsonData.c_str(), jsonData.length() + 1) < 0) {
        perror("Error writing JSON to server");
        return;
    }

    // Receive and print server response
    char buffer[1024] = {0};
    int val_read = read(clientSocket, buffer, sizeof(buffer));
    if (val_read < 0) {
        perror("Error reading response from server");
    } else {
        std::cout << "Server response:\n" << buffer << std::endl;
    }
}

void Client::retrieveSentMessages(const std::string& requestType) {
    // Send requestType, sender, receiver(NULL), and message(NULL) to server by JSON
    std::string jsonData = JsonHandler::serializeToJson(requestType, username, "", "");
    if (write(clientSocket, jsonData.c_str(), jsonData.length() + 1) < 0) {
        perror("Error writing JSON to server");
        return;
    }

    // Receive and print server response
    char buffer[1024] = {0};
    int val_read = read(clientSocket, buffer, sizeof(buffer));
    if (val_read < 0) {
        perror("Error reading response from server");
    } else {
        std::cout << "Server response:\n" << buffer << std::endl;
    }
}

void Client::retrieveReceivedMessages(const std::string& requestType) {
    // Send requestType, sender(NULL), receiver, and message(NULL) to server by JSON
    std::string jsonData = JsonHandler::serializeToJson(requestType, "", username, "");
    if (write(clientSocket, jsonData.c_str(), jsonData.length() + 1) < 0) {
        perror("Error writing JSON to server");
        return;
    }

    // Receive and print server response
    char buffer[1024] = {0};
    int val_read = read(clientSocket, buffer, sizeof(buffer));
    if (val_read < 0) {
        perror("Error reading response from server");
    } else {
        std::cout << "Server response:\n" << buffer << std::endl;
    }
}

void Client::quitProgram(const std::string& requestType) {
    // Send requestType, sender(NULL), receiver(NULL), and message(NULL) to server by JSON
    std::string jsonData = JsonHandler::serializeToJson(requestType, "", "", "");
    if (write(clientSocket, jsonData.c_str(), jsonData.length() + 1) < 0) {
        perror("Error writing JSON to server");
        return;
    }

    // Receive and print server response
    char buffer[1024] = {0};
    int val_read = read(clientSocket, buffer, sizeof(buffer));
    if (val_read < 0) {
        perror("Error reading response from server");
    } else {
        std::cout << "Server response:\n" << buffer << std::endl;
    }

    // Close socket
    close(clientSocket);
    exit(0); // Quit program
}
