#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <iostream>
#include <thread>
#include <atomic>
#include <sstream> 
#include <vector> 
#include <string>   
#include "CubeData.h"
#include <mutex>






#pragma comment(lib, "ws2_32.lib")

#define DEFAULT_BUFLEN 512

class NetworkManager
{
public:
    NetworkManager();
    ~NetworkManager();

    bool Initialize(bool isServer, std::string ip = "127.0.0.1", int port = 54000);
    bool Disconnect();
    bool ConnectToServer();
    bool AcceptConnection();
    bool SendData(const std::string& message);
    bool SendHello();
    std::string ListenForMessages();

    void StartListeningThread();   
    void StopListeningThread();
    bool SendCubeData(int id, bool isHit);
    std::pair<int, bool> ReceiveCubeData();
    std::string FormatCubeDataMessage(const CubeData& cubeData);
    bool ParseCubeDataMessage(const std::string& message, CubeData& cubeData);


    std::vector<CubeData> receivedCubeData; // Shared data
    std::mutex cubeDataMutex;               // Mutex to protect shared data

private:
    WSADATA wsaData;
    SOCKET ConnectionSocket;
    SOCKET ListenSocket;
    sockaddr_in serverAddress;
    bool initialized;
    bool isServer;
    bool running;
    std::thread listeningThread;
    std::unique_ptr<std::thread> listenerThread; 
    std::atomic<bool> shouldStopListening = false; 
    std::string SerializeCubeData(int id, bool isHit);
    std::pair<int, bool> ParseCubeData(const std::string& data);


};
