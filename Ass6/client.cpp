#include <iostream>
#include <winsock2.h>
#include <string>
using namespace std;

#pragma comment(lib, "ws2_32.lib")

int main()
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET clientSocket;
    sockaddr_in serverAddr;

    char buffer[1024];

    // 1. Create UDP socket
    clientSocket = socket(AF_INET, SOCK_DGRAM, 0);

    // 2. Configure server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // 3. Input string
    string message;

    cout << "Enter a string: ";
    getline(cin, message);

    // 4. Send string to server
    sendto(
        clientSocket,
        message.c_str(),
        message.length(),
        0,
        (sockaddr*)&serverAddr,
        sizeof(serverAddr)
    );

    cout << "Message sent to server!" << endl;

    // 5. Receive reversed string
    int bytesReceived = recvfrom(
        clientSocket,
        buffer,
        sizeof(buffer) - 1,
        0,
        nullptr,
        nullptr
    );

    if (bytesReceived >= 0)
    {
        buffer[bytesReceived] = '\0';

        // 6. Display server reply
        cout << "Reversed string: "
             << buffer << endl;
    }

    // 7. Close socket
    closesocket(clientSocket);

    // 8. Clean up Winsock
    WSACleanup();

    return 0;
}