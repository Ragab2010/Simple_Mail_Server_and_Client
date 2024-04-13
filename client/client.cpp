#include "ClientHeader.h"
#include <iostream>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <username>" << std::endl;
        return 1;
    }

    std::string username = argv[1];
    Client client(username);

    while (true) {
        std::cout << "\nMenu:\n";
        std::cout << "1. Send a message\n";
        std::cout << "2. Retrieve sent messages\n";
        std::cout << "3. Retrieve received messages\n";
        std::cout << "4. Quit\n";
        std::cout << "Enter your choice: ";

        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1:
            case 's':
                client.sendMessage("send");
                continue;
            case 2:
            case 'w':
                client.retrieveSentMessages("retrieve_sent");
                continue;
            case 3:
            case 'r':
                client.retrieveReceivedMessages("retrieve_received");
                continue;
            case 4:
            case 'q':
                client.quitProgram("quit");
                continue;
            default:
                std::cerr << "Invalid choice. Please try again." << std::endl;
                continue;
        }
    }

    return 0;
}
