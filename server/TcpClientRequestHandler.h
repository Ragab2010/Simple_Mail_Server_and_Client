#ifndef TCP_CLIENT_REQUEST_HANDLER_H
#define TCP_CLIENT_REQUEST_HANDLER_H


#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include "SQLiteDatabase.h"
#include "JsonHandler.h"

class TcpClientRequestHandler {
private:
    // SQLiteDatabase& db;
    // int clientSocket;
    // std::string username;

public:

    void handleSendRequest(int clientSocket, SQLiteDatabase& db , std::string sender, std::string receiver,std::string message);
    void handleRetrieveSentRequest(int clientSocket, SQLiteDatabase& db , std::string sender) ;
    void handleRetrieveReceivedRequest(int clientSocket, SQLiteDatabase& db , std::string receiver) ;
    void handleQuitRequest(int clientSocket , std::string sender) ;

};

#endif // TCP_CLIENT_REQUEST_HANDLER_H
