#pragma once

#include <functional>
#include <thread>
#include <vector>
#include <atomic>

class ThreadPool {
    std::vector<std::thread> _worker_threads;
    std::atomic_bool& _continue;

public:
    ThreadPool() = delete;
    ThreadPool(std::atomic_bool& do_continue, const std::function<void()>& task, const unsigned int threads);
    auto ShutDown() -> void;

    ~ThreadPool();

private:
    auto TaskSpinner(const std::function<void()>& task) -> void;
};