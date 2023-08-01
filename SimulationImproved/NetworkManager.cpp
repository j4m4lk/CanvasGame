#include "NetworkManager.h"

NetworkManager::NetworkManager() : ConnectionSocket(INVALID_SOCKET), ListenSocket(INVALID_SOCKET), initialized(false), isServer(false),running(false) {}

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

    // We've connected to the server, so start the listening thread
    StartListeningThread();

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

    // We've accepted a connection, so start the listening thread
    StartListeningThread();

    return true;
}

bool NetworkManager::Disconnect() {
    // Stop the listening thread before we disconnect
    StopListeningThread();

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


bool NetworkManager::SendData(const std::string& message) {
    char buffer[100];
    sprintf_s(buffer, "SEnding data \n ");
    OutputDebugStringA(buffer);
    std::string debugMsg = "Sending data: " + message + "\n";
    OutputDebugStringA(debugMsg.c_str());

    int iResult = send(ConnectionSocket, message.c_str(), message.size(), 0);
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectionSocket);
        return false;
    }
    return true;
}

bool NetworkManager::SendHello() {
    std::string helloMsg = "HELLO";
    return SendData(helloMsg);
}

std::string NetworkManager::ListenForMessages() {
    
   
    char buffer[DEFAULT_BUFLEN];
    int result = recv(ConnectionSocket, buffer, DEFAULT_BUFLEN, 0);
    if (result > 0) {
        std::string receivedData(buffer);
        std::string debugMsg = "Received data: " + receivedData + "\n";
        OutputDebugStringA(debugMsg.c_str());
        if (receivedData == "HELLO") {
            SendData("ACK");
            sprintf_s(buffer, "Hellow recieved \n");
            OutputDebugStringA(buffer);
        }
        else {
            //// Handle CubeData messages
            //CubeData cubeData;
            //if (ParseCubeDataMessage(receivedData, cubeData)) {
            //    std::lock_guard<std::mutex> lock(cubeDataMutex); 
            //    receivedCubeData.push_back(cubeData);
            //
            //}
        }
        return receivedData;
    }
    else if (result == 0)
        std::cout << "Connection closed\n";
    else
        std::cout << "recv failed with error: " << WSAGetLastError() << "\n";

    return "";
}
void NetworkManager::StartListeningThread() {
    running = true;
    listeningThread = std::thread([this]() {
        while (running) {
            ListenForMessages();
        }
        });
}

void NetworkManager::StopListeningThread() {
    running = false;
    if (listeningThread.joinable()) {
        listeningThread.join();
    }
}

// Helper function to serialize the data.
std::string NetworkManager::SerializeCubeData(int id, bool isHit) {
    std::stringstream ss;
    ss << "CubeData:" << id << ":" << isHit;
    return ss.str();
}

// Send cube data over the network.
bool NetworkManager::SendCubeData(int id, bool isHit) {
    std::string data = SerializeCubeData(id, isHit);
    return SendData(data);
}

// Parse received data into cube ID and isHit status.
std::pair<int, bool> NetworkManager::ParseCubeData(const std::string& data) {
    std::stringstream ss(data);
    std::string item;
    std::vector<std::string> tokens;

    while (std::getline(ss, item, ':')) {
        tokens.push_back(item);
    }

    if (tokens.size() == 3 && tokens[0] == "CubeData") {
        int id = std::stoi(tokens[1]);
        bool isHit = (tokens[2] == "1");
        return std::make_pair(id, isHit);
    }

    return std::make_pair(-1, false);  // Indicate an error.
}

// Receive cube data over the network.
std::pair<int, bool> NetworkManager::ReceiveCubeData() {
    std::string receivedData = ListenForMessages();

    if (!receivedData.empty()) {
        return ParseCubeData(receivedData);
    }

    return std::make_pair(-1, false);  // Indicate an error.
}
std::string NetworkManager::FormatCubeDataMessage(const CubeData& cubeData) {
    std::stringstream ss;
    ss << cubeData.id << ":" << (cubeData.isHit ? "1" : "0");
    return ss.str();
}
bool NetworkManager::ParseCubeDataMessage(const std::string& message, CubeData& cubeData) {
    std::stringstream ss(message);
    std::string item;
    std::getline(ss, item, ':');
    if (ss.fail())
        return false;
    cubeData.id = std::stoi(item);
    std::getline(ss, item);
    if (ss.fail())
        return false;
    cubeData.isHit = (item == "1");
    return true;
}


