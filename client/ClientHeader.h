#ifndef CLIENT_HEADER_H
#define CLIENT_HEADER_H

#include <string>

class Client {
public:
    Client(const std::string& username);
    void sendMessage(const std::string& requestType);
    void retrieveSentMessages(const std::string& requestType);
    void retrieveReceivedMessages(const std::string& requestType);
    void quitProgram(const std::string& requestType);

private:
    int clientSocket;
    std::string username;
};

#endif /* CLIENT_HEADER_H */
