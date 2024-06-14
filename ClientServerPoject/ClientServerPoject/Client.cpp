// client.cpp
#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>


#pragma comment(lib, "Ws2_32.lib")

int main() {
    std::cout<<"enter some chars"<<std::endl;
    std::string chars;
    std::cin>>chars;
    // Communicate with the server (send/receive data)
    //if the message is a string
    char chararr[10];


    
    WSADATA mysocketdata;
    if (WSAStartup(MAKEWORD(2, 2), &mysocketdata) != 0) {
        std::cerr << "wsa object not created" << WSAGetLastError() << std::endl;
        return 1;
    }

    // Create a client socket
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Error creating socket. Error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }
    // std::cout << "Enter Some Characters" << std::endl;
    
    
  
    sockaddr_in clientAddress;
    clientAddress.sin_family = AF_INET;
    clientAddress.sin_port = htons(8080); // Ensure this matches the server port

    // Convert IP address from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &clientAddress.sin_addr) <= 0) {
        std::cerr << "Invalid address or address not supported. Error: " << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    // Connect to the server
    if (connect(clientSocket, reinterpret_cast<sockaddr*>(&clientAddress), sizeof(clientAddress)) == SOCKET_ERROR) {
        std::cerr << "client failed to connect to server, Error: " << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }
   


    const std::string message = chars;
    for(int i = 0; i < chars.length(); i++)
    {
        chararr[i] =chars.at(i) ;    
    }
    

    
    if (send(clientSocket, chararr, strlen(chararr), 0) == SOCKET_ERROR) {
        std::cerr << "Send failed. Error: " << WSAGetLastError() << std::endl;
    }
    //closesocket(clientSocket);

   
//check if the bytes where recieved correctly
    char buffer[1024] = {};
    const auto bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived > 0) {
        std::cout << "Server says: " << buffer << std::endl;
        buffer[bytesReceived] = '\0';
    }
    else {
        std::cerr << "Recv failed. Error: " << WSAGetLastError() << std::endl;
    }

    // Clean up
    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
