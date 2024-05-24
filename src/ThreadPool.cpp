#include "ThreadPool.h"

ThreadPool::ThreadPool(std::atomic_bool& do_continue, const std::function<void()>& task, const unsigned int threads)
: _continue{do_continue}
{
    // const unsigned int core_count {std::thread::hardware_concurrency()};
    for (int i = 0; i < threads; ++i) {
        _worker_threads.push_back(std::thread{std::bind(&ThreadPool::TaskSpinner, this, task)});
    }
}

auto ThreadPool::ShutDown() -> void {
    for (auto& worker : _worker_threads) {
        worker.join();
    }
}

ThreadPool::~ThreadPool() {
    ShutDown();
}


auto ThreadPool::TaskSpinner(const std::function<void()>& task) -> void {
    while (_continue) {
        task();
    }
    return;
}