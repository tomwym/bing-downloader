#pragma once

#include <filesystem>
#include <queue>
#include <mutex>
#include <memory>

#include "ThreadPool.h"
#include "NaiveSocketComponents.h"
#include "NaiveHeader.h"

using QueueOfSockets = std::queue<std::unique_ptr<Naive::NaiveSocketComponents>>;

class SimpleFoxServer {
    Naive::NaiveSocketComponents _server;
    std::atomic_bool        _continue;
    std::atomic_uint        _active_transfers;
    const std::vector<char> _response_content;
    const Header            _response_header;
    std::mutex              _queue_mutex;
    QueueOfSockets          _incoming_queue;
    ThreadPool              _worker_pool;
    std::thread             _distributor_thread;


public:
    SimpleFoxServer(const std::filesystem::path& fox_path);
    ~SimpleFoxServer();
    auto MainThreadSpinner() -> void;
private:
    auto Distribute() -> void;
    auto Task() -> void;
};