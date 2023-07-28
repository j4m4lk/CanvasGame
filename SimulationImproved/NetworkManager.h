#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

class NetworkManager
{
public:
    NetworkManager();
    ~NetworkManager();

    bool Initialize(bool isServer, std::string ip = "127.0.0.1", int port = 54000);
    bool Disconnect();
    bool ConnectToServer();
    bool AcceptConnection();

private:
    WSADATA wsaData;
    SOCKET ConnectionSocket;
    SOCKET ListenSocket;
    sockaddr_in serverAddress;
    bool initialized;
    bool isServer;
};
