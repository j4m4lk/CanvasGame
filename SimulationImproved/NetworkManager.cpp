#include "NetworkManager.h"

NetworkManager::NetworkManager() : ConnectSocket(INVALID_SOCKET), ListenSocket(INVALID_SOCKET) {}

NetworkManager::~NetworkManager() {
    closesocket(ConnectSocket);
    closesocket(ListenSocket);
    WSACleanup();
}

bool NetworkManager::Initialize() {
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cout << "WSAStartup failed with error: " << result << std::endl;
        return false;
    }
    else {
        char buffer[100];
        sprintf_s(buffer, "WSAStartup initialized successfully.\n");
        OutputDebugStringA(buffer);
    }

    ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ConnectSocket == INVALID_SOCKET) {
        std::cout << "Socket creation failed with error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return false;
    }
    else {
        char buffer[100];
        sprintf_s(buffer, "Socket created successfully.\n");
        OutputDebugStringA(buffer);
    }

    return true;
}

bool NetworkManager::Connect() {
    // Hardcoded local loopback address and port
    std::string m_ip = "127.0.0.1";
    int m_port = 55555; // Choose any valid port that's not in use 

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(m_port);
    InetPtonA(AF_INET, m_ip.c_str(), &serverAddress.sin_addr); // Change to InetPtonA

    int result = connect(ConnectSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress));
    if (result == SOCKET_ERROR) {
        std::cout << "Connection failed with error: " << WSAGetLastError() << std::endl;
        closesocket(ConnectSocket);
        WSACleanup();
        return false;
    }

    return true;
}



bool NetworkManager::InitServer() {
    int m_port = 55555;
    ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ListenSocket == INVALID_SOCKET) {
        std::cout << "Socket creation failed with error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return false;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(m_port); // use the same port as the client
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    int result = bind(ListenSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress));
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

    return true;
}

bool NetworkManager::AcceptConnection() {
    ConnectSocket = accept(ListenSocket, NULL, NULL);
    if (ConnectSocket == INVALID_SOCKET) {
        std::cout << "Accepting connection failed with error: " << WSAGetLastError() << std::endl;
        closesocket(ListenSocket);
        WSACleanup();
        return false;
    }

    return true;
}

bool NetworkManager::Disconnect() {
    int iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        closesocket(ListenSocket);
        WSACleanup();
        return false;
    }

    closesocket(ConnectSocket);
    closesocket(ListenSocket);
    WSACleanup();
    return true;
}

bool NetworkManager::LoadServerInfoFromFile() {
    std::ifstream file("config.txt");
    if (file.is_open()) {
        std::getline(file, m_ip);
        file >> m_port;
        file.close();
        return true;
    }
    return false;
}
