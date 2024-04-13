
#include <fstream>
#include <sstream>
#include "HttpClientRequestHandle.h"

static std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}
void HttpClientRequestHandler::sendHttpPostTextResponse(int socket, const std::string& response) {
    std::string httpResponse =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: " + std::to_string(response.length()) + "\r\n"
        "\r\n" + response;
    //std::cout<<"httpResponse:"<<httpResponse<<std::endl;
    write(socket, httpResponse.c_str(), httpResponse.length()+1);
}
void HttpClientRequestHandler::sendHttpPostJsonResponse(int socket, const std::string& response) {
    std::string httpResponse =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: " + std::to_string(response.length()) + "\r\n"
        "\r\n" + response;

    write(socket, httpResponse.c_str(), httpResponse.length()+1);
}
void HttpClientRequestHandler::sendHttpGetTextResponse(int socket, const std::string& response) {
    std::string httpResponse =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: " + std::to_string(response.length()) + "\r\n"
        "\r\n" + response;

    write(socket, httpResponse.c_str(), httpResponse.length()+1);
}

void HttpClientRequestHandler::handleGetRequest(std::string fileName , int clientSocket){
        
    // Serve index.html if no specific file requested
    if (fileName == "/" || fileName == "/index.html") {
        fileName = "index.html";
        std::string fileContent = readFile(fileName);

        if (fileContent.empty()) {
            std::string notFoundResponse =
                "HTTP/1.1 404 Not Found\r\n"
                "Content-Type: text/plain\r\n"
                "Content-Length: 13\r\n"
                "\r\n"
                "404 Not Found";
            write(clientSocket, notFoundResponse.c_str(), notFoundResponse.length()+1);
        } else {
            sendHttpGetTextResponse(clientSocket, fileContent);
        }
    }


}
void HttpClientRequestHandler::handleSendRequest(int clientSocket, SQLiteDatabase& db , std::string sender, std::string receiver,std::string message) {

    // Store the message in the database
    db.storeMessage(sender.c_str(), receiver.c_str(), message.c_str());

    // Send response to client
    std::string response_message= "Message stored successfully.";
    sendHttpPostTextResponse(clientSocket, response_message);
    // if (write(clientSocket, response_message.c_str(), response_message.length()+1) < 0) {
    //     perror("Error writing response_message to client");
    // }
    //close(clientSocket);
}

void HttpClientRequestHandler::handleRetrieveSentRequest(int clientSocket, SQLiteDatabase& db , std::string sender) {

    // Retrieve sent messages for the given username from the database
    std::string sentMessages = db.retrieveSentMessages(sender.c_str());

    // Send sent messages to client
    sendHttpPostTextResponse(clientSocket, sentMessages);
    // if (write(clientSocket, sentMessages.c_str(), sentMessages.length()+1) < 0) {
    //     perror("Error writing (Retrieve Sent Message) to client");
    // }
    //close(clientSocket);
}

void HttpClientRequestHandler::handleRetrieveReceivedRequest(int clientSocket, SQLiteDatabase& db , std::string receiver) {

    // Retrieve received messages for the given username from the database
    std::string receivedMessages = db.retrieveReceivedMessages(receiver.c_str());

    // Send received messages to client
    sendHttpPostTextResponse(clientSocket, receivedMessages);
    // if (write(clientSocket, receivedMessages.c_str(), receivedMessages.length()+1) < 0) {
    //     perror("Error writing (Retrieve Received Message) to client");
    // }
    //close(clientSocket);
}

void HttpClientRequestHandler::handleQuitRequest(int clientSocket , std::string sender) {
    // Send response to client
    // if (write(clientSocket, "Goodbye!", 9) < 0) {
    //     perror("Error writing to client");
    // }

    sendHttpPostTextResponse(clientSocket, "Goodbye!");
    // Print the client Connected
    close(clientSocket);
}


