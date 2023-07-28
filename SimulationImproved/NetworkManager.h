#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <fstream>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

class NetworkManager
{
public:
    NetworkManager();
    ~NetworkManager();
    bool Initialize();
    bool Connect();
    bool Disconnect();
    bool InitServer();
    bool AcceptConnection();

private:
    bool LoadServerInfoFromFile();

    WSADATA wsaData;
    SOCKET ConnectSocket;
    SOCKET ListenSocket;
    std::string m_ip;
    int m_port;
};
