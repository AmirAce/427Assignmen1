// server.cpp
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;
void toCap(char* buffer, int length) {
    for (int i = 0; i < length; ++i) {
        buffer[i] = toupper(buffer[i]);
    }
}



int main() 
{
    //create a wsadata object
    WSADATA mysockeddata;
    //check if it has the correct version
    if (WSAStartup(MAKEWORD(2, 2), &mysockeddata) != 0) {
        cerr << "WSAStartup object not created,  Error: " << WSAGetLastError() << endl;
        return 1;
    }

    // Creating socket
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0); 
    if (serverSocket == INVALID_SOCKET) {
        cerr << "Error creating socket. Error: " << WSAGetLastError() << endl;
        WSACleanup();
        return 1;
    }

    // Specifying the address
    sockaddr_in serverAddress; 
    serverAddress.sin_family = AF_INET; 
    serverAddress.sin_port = htons(8080); 
    serverAddress.sin_addr.s_addr = INADDR_ANY; 

    // Binding socket
    if (bind(serverSocket, reinterpret_cast<struct sockaddr*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR) {
        cerr << "Bind failed. Error: " << WSAGetLastError() << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Listening to the assigned socket
    if (listen(serverSocket, 5) == SOCKET_ERROR) {
        cerr << "Listen failed. Error: " << WSAGetLastError() << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    
    cout << "Server is listening on port 8080..." << endl;

    // Accepting connection request
    SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);



    // are we recieving the data correctly?
    char buffer[1024] = {}; 
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    //check if data is empty or not
    if (bytesReceived > 0) {
        cout << "client says: " << buffer << endl; 
        // Send response to the client
        toCap(buffer, bytesReceived);

        
        
        if(send(clientSocket, buffer, std::strlen(buffer), 0) == SOCKET_ERROR)
        {
            std::cerr << "Send failed. Error: " << WSAGetLastError() << std::endl;
        }
            
    } else {
        cerr << "Recv failed. Error: " << WSAGetLastError() << endl;
    }

    // quit
    closesocket(clientSocket);

    //shutdown
    closesocket(serverSocket);

    WSACleanup();
    return 0; 
}

