#include "CurlGetDispatcher.h"

#include <format>
#include <iostream>

CurlGetDispatcher::CurlGetDispatcher(const bool test_run, const unsigned int threads)
: _test_run{test_run}
, _continue{true}
, _queue_mutex{}
, _queue{}
, _workers{std::make_unique<ThreadPool>(_continue, std::bind(&CurlGetDispatcher::Task, this), threads)}
{}

CurlGetDispatcher::~CurlGetDispatcher() {
    bool queue_empty;
    do {
        std::lock_guard<std::mutex> lock{_queue_mutex};
        queue_empty = _queue.empty();
    } while(!queue_empty);
    _continue = false;
}

auto CurlGetDispatcher::Add(std::unique_ptr<DownloadInfo>&& task) -> void {
    std::lock_guard<std::mutex> lock{_queue_mutex};
    _queue.emplace(std::move(task));
}

auto CurlGetDispatcher::Task() -> void {
    std::unique_ptr<DownloadInfo> info;
    {
        std::lock_guard<std::mutex> lock{_queue_mutex};
        if (_queue.empty()) {
            return;
        }
        info = std::move(_queue.front());
        _queue.pop();
    }
    const std::string server { _test_run ? "0.0.0.0:8080" : info->url};
    const std::filesystem::path destination_file{info->write_destination};
    CurlWriteChunks curl {server, destination_file};
    const std::uintmax_t filesize{std::filesystem::file_size(destination_file)};
    const unsigned int threshold{1024};
    if (curl.Success() && filesize < threshold) {
        std::cout << std::format("Written less than threshold {} at {} bytes. Removing file: {}.\n", threshold, filesize, destination_file.string());
        std::filesystem::remove(destination_file);
    }

}