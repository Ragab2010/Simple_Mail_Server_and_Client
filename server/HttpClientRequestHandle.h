#ifndef HTTP_CLIENT_REQUEST_HANDLER_H
#define HTTP_CLIENT_REQUEST_HANDLER_H


#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include "SQLiteDatabase.h"
#include "JsonHandler.h"

class HttpClientRequestHandler{
private:
    // SQLiteDatabase& db;
    // int clientSocket;
    // std::string username;

public:

    void sendHttpPostJsonResponse(int socket, const std::string& response);
    void sendHttpPostTextResponse(int socket, const std::string& response);
    void sendHttpGetTextResponse(int socket, const std::string& response);

    void handleGetRequest(std::string fileName , int clientSocket);
    void handleSendRequest(int clientSocket, SQLiteDatabase& db , std::string sender, std::string receiver,std::string message);
    void handleRetrieveSentRequest(int clientSocket, SQLiteDatabase& db , std::string sender) ;
    void handleRetrieveReceivedRequest(int clientSocket, SQLiteDatabase& db , std::string receiver) ;
    void handleQuitRequest(int clientSocket , std::string sender) ;

};

#endif // HTTP_CLIENT_REQUEST_HANDLER_H
