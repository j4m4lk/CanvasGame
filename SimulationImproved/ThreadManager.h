#pragma once
#include <thread>
#include <functional>
#include <atomic>
#include <windows.h>


class ThreadManager {
public:
    ThreadManager();
    ~ThreadManager();

    void StartThread(std::function<void()> function, DWORD_PTR processorIndex);
    void StopThread();
    bool ShouldStop() const;

private:
    std::unique_ptr<std::thread> thread;
    std::atomic<bool> shouldStop;
};
