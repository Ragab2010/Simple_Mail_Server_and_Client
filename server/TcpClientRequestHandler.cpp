
#include "TcpClientRequestHandler.h"

void TcpClientRequestHandler::handleSendRequest(int clientSocket, SQLiteDatabase& db , std::string sender, std::string receiver,std::string message) {

    // Store the message in the database
    db.storeMessage(sender.c_str(), receiver.c_str(), message.c_str());

    // Send response to client
    std::string response_message= "Message stored successfully.";
    if (write(clientSocket, response_message.c_str(), response_message.length()+1) < 0) {
        perror("Error writing response_message to client");
    }
    //close(clientSocket);
}

void TcpClientRequestHandler::handleRetrieveSentRequest(int clientSocket, SQLiteDatabase& db , std::string sender) {

    // Retrieve sent messages for the given username from the database
    std::string sentMessages = db.retrieveSentMessages(sender.c_str());

    // Send sent messages to client
    if (write(clientSocket, sentMessages.c_str(), sentMessages.length()+1) < 0) {
        perror("Error writing (Retrieve Sent Message) to client");
    }
    //close(clientSocket);
}

void TcpClientRequestHandler::handleRetrieveReceivedRequest(int clientSocket, SQLiteDatabase& db , std::string receiver) {

    // Retrieve received messages for the given username from the database
    std::string receivedMessages = db.retrieveReceivedMessages(receiver.c_str());

    // Send received messages to client
    if (write(clientSocket, receivedMessages.c_str(), receivedMessages.length()+1) < 0) {
        perror("Error writing (Retrieve Received Message) to client");
    }
    //close(clientSocket);
}

void TcpClientRequestHandler::handleQuitRequest(int clientSocket , std::string sender) {
    // Send response to client
    if (write(clientSocket, "Goodbye!", 9) < 0) {
        perror("Error writing to client");
    }
    // Print the client Connected
    std::cout << sender << " Disconnect...." << std::endl;
    close(clientSocket);
}


