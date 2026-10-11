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
    sockaddr_in server;
    char buffer[1024];

    // 1. Create TCP socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    // 2. Set server IP and port
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    // 3. Connect to server
    if (connect(
        clientSocket,
        (sockaddr*)&server,
        sizeof(server)
    ) == SOCKET_ERROR)
    {
        cout << "Connection failed!" << endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    cout << "Connected to server!" << endl;

    // 4. Communication loop
    while (true)
    {
        string message;

        // Enter message
        cout << "\nYou: ";
        getline(cin, message);

        // Send message to server
        send(
            clientSocket,
            message.c_str(),
            message.length(),
            0
        );

        // If client says bye, receive closing message
        if (message == "bye")
        {
            int bytesReceived = recv(
                clientSocket,
                buffer,
                sizeof(buffer) - 1,
                0
            );

            if (bytesReceived > 0)
            {
                buffer[bytesReceived] = '\0';
                cout << "\nServer: " << buffer << endl;
            }

            break;
        }

        // Receive server reply
        int bytesReceived = recv(
            clientSocket,
            buffer,
            sizeof(buffer) - 1,
            0
        );

        if (bytesReceived <= 0)
        {
            cout << "Server disconnected." << endl;
            break;
        }

        buffer[bytesReceived] = '\0';

        // Display server reply
        cout << "Server: " << buffer << endl;
    }

    // 5. Close socket
    closesocket(clientSocket);

    // 6. Clean up Winsock
    WSACleanup();

    cout << "Client terminated." << endl;

    return 0;
}