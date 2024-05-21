#include "../CurlWriteChunks.h"
#include <filesystem>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <sstream>
#include <atomic>
#include <thread>

std::atomic_int counter{0};


std::string getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm local_tm = *std::localtime(&now_time_t);
    auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    std::ostringstream oss;
    oss << std::put_time(&local_tm, "%y%m%d_%H%M%S");
    oss << std::setw(3) << std::setfill('0') << now_ms.count();
    return oss.str();
}

int main () {
    counter = 0;
    const std::filesystem::path temp_dir{ ".temp" };
    for (const auto& entry : std::filesystem::directory_iterator(temp_dir))
        std::filesystem::remove_all(entry.path());

    std::vector<std::thread> pool;

    auto Task = [&]() {
        for (int i = 0; i<5; ++i) {
            auto redo = false;
            do {
                CurlWriteChunks curl {"0.0.0.0:8080", temp_dir / std::format("fox_{}_{}.jpg", counter++, getCurrentTimestamp())};
                if (!curl.Success()) {
                    std::cout << "ERR on " << std::this_thread::get_id() << '\n';
                    redo = true;
                }
                else {
                    redo = false;
                }
            } while (redo);

        }
    };

    const unsigned int thread_count {4};
    for (int i = 0; i<thread_count; ++i) {
        pool.emplace_back(std::thread{Task});
    }

    for (auto& t : pool) {
        t.join();
    }

    // CurlWriteChunks curl {"0.0.0.0:8080", temp_dir / std::format("fox_{}.jpg", getCurrentTimestamp())};
    return 0;
}