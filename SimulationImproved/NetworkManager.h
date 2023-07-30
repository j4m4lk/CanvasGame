#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <iostream>
#include <thread>
#include <atomic>

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

    // Starts the background thread that listens for messages from the server.
    void StartListeningThread();

    // Stops the background thread that listens for messages from the server.
    void StopListeningThread();

private:
    WSADATA wsaData;
    SOCKET ConnectionSocket;
    SOCKET ListenSocket;
    sockaddr_in serverAddress;
    bool initialized;
    bool isServer;
    bool running;
    std::thread listeningThread;
    std::unique_ptr<std::thread> listenerThread; // For managing the thread object
    std::atomic<bool> shouldStopListening = false; // Flag to control when the thread should stop
    //...

};
