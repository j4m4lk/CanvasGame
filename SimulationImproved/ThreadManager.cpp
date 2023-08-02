#include "ThreadManager.h"

ThreadManager::ThreadManager()
    : shouldStop(false) {
}

ThreadManager::~ThreadManager() {
    StopThread();
}

void ThreadManager::StartThread(std::function<void()> function, DWORD_PTR processorIndex) {
    shouldStop = false;

    thread = std::make_unique<std::thread>([this, function, processorIndex]() {
        // Seting thread affinity here
        DWORD_PTR mask = 1i64 << processorIndex;
    SetThreadAffinityMask(GetCurrentThread(), mask);

    while (!shouldStop) {
        function();
    }
        });

}

void ThreadManager::StopThread() {
    shouldStop = true;
    if (thread->joinable()) {
        thread->join();
    }
    thread.reset();
}

bool ThreadManager::ShouldStop() const {
    return shouldStop;
}
