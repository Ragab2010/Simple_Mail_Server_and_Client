#include "ServerSocket.h"
// #include "SQLiteDatabase.h"
// #include "JsonHandler.h"
#include <thread>
#include <cstring>
#include "TcpClientRequestHandler.h"
#include "HttpClientRequestHandle.h"

void httpClientHandler(int clientSocket,HttpClientRequestHandler &http,  SQLiteDatabase& db , std::string jsonData) {

    // DeSerialize parameters from JSON [requestType ,sender ,receiver , message ]
    auto [requestType ,sender ,receiver , message ] = JsonHandler::deserializeFromJson(jsonData);
    // std::cout<<"requestType: "<<requestType<<std::endl;
    // std::cout<<"sender: "<<sender<<std::endl;
    // std::cout<<"receiver: "<<receiver<<std::endl;
    // std::cout<<"message: "<<message<<std::endl;

    // Process client's choice
    if (requestType == "" && !sender.empty()) {
        // Print the client Connected
        std::cout << sender << " Connected...." << std::endl;
        std::string json_response = R"({})";
        http.sendHttpPostJsonResponse(clientSocket , json_response);
    } else if (requestType == "send") {
        http.handleSendRequest(clientSocket, db ,sender ,receiver , message );
    } else if (requestType == "retrieve_sent") {
        http.handleRetrieveSentRequest(clientSocket, db , sender);
    } else if (requestType == "retrieve_received") {
        http.handleRetrieveReceivedRequest(clientSocket, db ,receiver );
    } else if (requestType == "quit") {
        http.handleQuitRequest(clientSocket , sender);
        // Print the client Connected
        std::cout << sender << " Disconnect...." << std::endl;
    } else {
        // Invalid choice
        perror("Error writing (Invalid choice) to Web client");
    }



}


void tcpClientHandler(int clientSocket,TcpClientRequestHandler &tcpClient,  SQLiteDatabase& db , std::string username) {

    // Loop to handle client requests
    while (true) {

        // receive requestType ,sender, receiver, and message from client by json
        // receive JSON data from client
        char jsonData_buff[1024] = {0};
        if ( read(clientSocket, jsonData_buff, sizeof(jsonData_buff))< 0){
            perror("Error read json from client");
            break;
        }
        std::string jsonData(jsonData_buff);

        // DeSerialize parameters from JSON [requestType ,sender ,receiver , message ]
        auto [requestType ,sender ,receiver , message ] = JsonHandler::deserializeFromJson(jsonData);
        // Process client's choice
        if (requestType == "send") {
            tcpClient.handleSendRequest(clientSocket, db ,sender ,receiver , message );
        } else if (requestType == "retrieve_sent") {
            tcpClient.handleRetrieveSentRequest(clientSocket, db , sender);
        } else if (requestType == "retrieve_received") {
            tcpClient.handleRetrieveReceivedRequest(clientSocket, db ,receiver );
        } else if (requestType == "quit") {
            tcpClient.handleQuitRequest(clientSocket , username);
            break; // Exit loop when client quits
        } else {
            // Invalid choice
            if (write(clientSocket, "Invalid choice.", strlen("Invalid choice.") + 1) < 0) {
                perror("Error writing (Invalid choice) to client");
            }
        }
    }

}

void clientHandler(int clientSocket, SQLiteDatabase& db) {
    // Receive username from client
    char usernameBuffer_get_post_request[1024] = {0};
    // if (read(clientSocket, usernameBuffer_getRequest, sizeof(usernameBuffer_getRequest)) < 0) {
    if (read(clientSocket, usernameBuffer_get_post_request, sizeof(usernameBuffer_get_post_request) ) < 0) {
        perror("Error reading (name of client) from client");
        close(clientSocket);
        return;
    }

    if( (strncmp(usernameBuffer_get_post_request, "GET", 3) == 0 ) ){
        // Extract requested file name
        std::string request(usernameBuffer_get_post_request);

        size_t start = request.find(' ') + 1;
        size_t end = request.find(' ', start);
        std::string filePath = request.substr(start, end - start);
        // std::cout<<"GET:"<<std::endl;
        // std::cout<<"filePath: "<<filePath<<std::endl;
        HttpClientRequestHandler http;
        http.handleGetRequest(filePath ,clientSocket );
    }else if ((strncmp(usernameBuffer_get_post_request, "POST", 4) == 0)){
        // Find the start of the JSON data in the request
        char* jsonStart = strstr(usernameBuffer_get_post_request, "\r\n\r\n");
        if (jsonStart == nullptr) {
            //std::cout<<"POST request="<<jsonStart<<std::endl;
            // handlePostRequest(jsonStart, clientSocket);
            std::cout << "Failed to find JSON data in POST request" << std::endl;
            close(clientSocket);
            return ;
        } 
        // std::cout<<"POST:"<<std::endl;
        // std::cout<<"filePath: "<<jsonStart<<std::endl;

        HttpClientRequestHandler http;
        httpClientHandler(clientSocket , http , db , jsonStart);

    }else{
        std::string username(usernameBuffer_get_post_request);
        // Print the client Connected
        std::cout << username << " Connected...." << std::endl;
        
        //create ClientRequestHandler
        TcpClientRequestHandler tcp;
        tcpClientHandler(clientSocket ,tcp ,db , username );

    }

    // Close client socket
    close(clientSocket);
}


int main() {
    // Set up server socket
    ServerSocket serverSocket(8080); // Change port number if needed
    serverSocket.init();

    // Set up SQLite database
    SQLiteDatabase db;
    db.openDatabase("email.db");
    db.createTable();

    while (true) {
        // Accept client connections and handle requests
        int clientSocket = serverSocket.acceptClient();

        std::thread(clientHandler, clientSocket , std::ref(db)).detach();

    }

    // Close server socket and database connection
    serverSocket.closeSocket();
    db.closeDatabase();

    return 0;
}
