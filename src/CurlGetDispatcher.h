#pragma once

#include <filesystem>
#include <queue>

#include "ThreadPool.h"
#include "CurlWriteChunks.h"

struct DownloadInfo {
    std::string url;
    std::filesystem::path write_destination;
};

class CurlGetDispatcher {
    bool _test_run;
    std::atomic_bool _continue;
    std::mutex _queue_mutex;
    std::queue<std::unique_ptr<DownloadInfo>> _queue;
    std::unique_ptr<ThreadPool> _workers;
public:
    CurlGetDispatcher(const bool test_run, const unsigned int threads);
    ~CurlGetDispatcher();
    auto Add(std::unique_ptr<DownloadInfo>&& task) -> void;
private:
    auto Task() -> void;
};