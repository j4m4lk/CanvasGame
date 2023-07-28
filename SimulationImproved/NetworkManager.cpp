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

NetworkManager::NetworkManager() : ConnectionSocket(INVALID_SOCKET), ListenSocket(INVALID_SOCKET), initialized(false), isServer(false) {}

NetworkManager::~NetworkManager() {
    if (initialized) {
        closesocket(ConnectionSocket);
        WSACleanup();
    }
}

bool NetworkManager::Initialize(bool isServer, std::string ip, int port) {
    if (initialized) {
        return true; // Initialization has already been done
    }
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cout << "WSAStartup failed with error: " << result << std::endl;
        return false;
    }

    this->isServer = isServer;

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    InetPtonA(AF_INET, ip.c_str(), &serverAddress.sin_addr);

    if (isServer) {
        ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (ListenSocket == INVALID_SOCKET) {
            std::cout << "Socket creation failed with error: " << WSAGetLastError() << std::endl;
            WSACleanup();
            return false;
        }

        result = bind(ListenSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress));
        if (result == SOCKET_ERROR) {
            std::cout << "Binding failed with error: " << WSAGetLastError() << std::endl;
            closesocket(ListenSocket);
            WSACleanup();
            return false;
        }

        result = listen(ListenSocket, SOMAXCONN);
        if (result == SOCKET_ERROR) {
            std::cout << "Listening failed with error: " << WSAGetLastError() << std::endl;
            closesocket(ListenSocket);
            WSACleanup();
            return false;
        }
    }
    else {
        ConnectionSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (ConnectionSocket == INVALID_SOCKET) {
            std::cout << "Socket creation failed with error: " << WSAGetLastError() << std::endl;
            WSACleanup();
            return false;
        }
    }

    initialized = true;
    return true;
}

bool NetworkManager::ConnectToServer() {
    if (!initialized || isServer) {
        return false;
    }

    int result = connect(ConnectionSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress));
    if (result == SOCKET_ERROR) {
        std::cout << "Connection failed with error: " << WSAGetLastError() << std::endl;
        closesocket(ConnectionSocket);
        WSACleanup();
        return false;
    }

    return true;
}

bool NetworkManager::AcceptConnection() {
    if (!initialized || !isServer) {
        return false;
    }

    ConnectionSocket = accept(ListenSocket, NULL, NULL);
    if (ConnectionSocket == INVALID_SOCKET) {
        std::cout << "Accepting connection failed with error: " << WSAGetLastError() << std::endl;
        closesocket(ListenSocket);
        WSACleanup();
        return false;
    }

    return true;
}

bool NetworkManager::Disconnect() {
    int iResult = shutdown(ConnectionSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectionSocket);
        WSACleanup();
        return false;
    }

    closesocket(ConnectionSocket);
    WSACleanup();
    return true;
}

