#include <iostream>
#include <winsock2.h>
#include <string>
using namespace std;

#pragma comment(lib, "ws2_32.lib")

int main()
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET serverSocket, clientSocket;
    sockaddr_in server, client;
    int clientSize = sizeof(client);
    char buffer[1024];

    // 1. Create TCP socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    // 2. Bind socket to IP and port
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8080);

    bind(serverSocket, (sockaddr*)&server, sizeof(server));

    // 3. Listen for client
    listen(serverSocket, 1);

    cout << "Server waiting for client..." << endl;

    // 4. Accept client connection
    clientSocket = accept(
        serverSocket,
        (sockaddr*)&client,
        &clientSize
    );

    cout << "Client connected!" << endl;

    // 5. Communication loop
    while (true)
    {
        // Receive message from client
        int bytesReceived = recv(
            clientSocket,
            buffer,
            sizeof(buffer) - 1,
            0
        );

        if (bytesReceived <= 0)
            break;

        buffer[bytesReceived] = '\0';
        string message = buffer;

        cout << "\nClient: " << message << endl;

        // Check whether client wants to leave
        if (message == "bye")
        {
            string closingMsg =
                "Client said bye. Server is closing.";

            send(
                clientSocket,
                closingMsg.c_str(),
                closingMsg.length(),
                0
            );

            cout << closingMsg << endl;
            break;
        }

        // Enter reply
        string reply;

        cout << "Enter reply: ";
        getline(cin, reply);

        // Send reply to client
        send(
            clientSocket,
            reply.c_str(),
            reply.length(),
            0
        );
    }

    // 6. Close sockets
    closesocket(clientSocket);
    closesocket(serverSocket);

    // 7. Clean up Winsock
    WSACleanup();

    cout << "\nServer terminated." << endl;

    return 0;
}