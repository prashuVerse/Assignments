#include <iostream>
#include <winsock2.h>
#include <string>
using namespace std;

#pragma comment(lib, "ws2_32.lib")

int main()
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET serverSocket;
    sockaddr_in serverAddr, clientAddr;
    int clientSize = sizeof(clientAddr);

    char buffer[1024];

    // 1. Create UDP socket
    serverSocket = socket(AF_INET, SOCK_DGRAM, 0);

    // 2. Configure server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8080);

    // 3. Bind socket to IP and port
    bind(
        serverSocket,
        (sockaddr*)&serverAddr,
        sizeof(serverAddr)
    );

    cout << "UDP Server waiting for client..." << endl;

    // 4. Receive message from client
    int bytesReceived = recvfrom(
        serverSocket,
        buffer,
        sizeof(buffer) - 1,
        0,
        (sockaddr*)&clientAddr,
        &clientSize
    );

    if (bytesReceived >= 0)
    {
        buffer[bytesReceived] = '\0';

        // 5. Display received message
        cout << "Received from client: "
             << buffer << endl;

        // 6. Reverse the string
        string message = buffer;
        string reversed = "";

        for (int i = message.length() - 1; i >= 0; i--)
        {
            reversed += message[i];
        }

        // 7. Send reversed string to client
        sendto(
            serverSocket,
            reversed.c_str(),
            reversed.length(),
            0,
            (sockaddr*)&clientAddr,
            clientSize
        );

        cout << "Reversed string sent: "
             << reversed << endl;
    }

    // 8. Close socket
    closesocket(serverSocket);

    // 9. Clean up Winsock
    WSACleanup();

    return 0;
}