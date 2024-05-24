#include "NaiveHeader.h"

#include <sstream>
#include <format>
#include <chrono>
#include <iomanip>



Header::operator std::string() const {
    return Serialize();
}

auto Header::AddAcceptRanges() -> Header& {
    static std::string key{"accept-ranges"};
    _content[key] = "bytes";

    return *this;
}


auto Header::AddDate() -> Header& {
    static std::string key{"date"};
    auto now = std::chrono::system_clock::now();
    std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm gmt_tm = *std::gmtime(&now_time_t);
    std::ostringstream oss;
    oss << std::put_time(&gmt_tm, "%a, %d %b %Y %H:%M:%S GMT");

    _content[key] = oss.str();
    return *this;
}

auto Header::AddContentType(const CONTENT_TYPE& content_type) -> Header& {
    static std::string key{"content-type"};
    static const std::map<CONTENT_TYPE, std::string> content_type_map {
        {CONTENT_TYPE::IMAGE_JPEG, "image/jpg"},
    };
    auto it = content_type_map.find(content_type);
    if (it != content_type_map.end())
        _content[key] = it->second;
    return *this;
}

auto Header::AddContentLength(const std::size_t& size) -> Header& {
    static std::string key{"content-length"};
    _content[key] = std::to_string(size);
    return *this;
}

auto Header::AddHost(const std::string& host) -> Header& {
    static std::string key{"host"};
    _content[key] = host;
    return *this;
}

auto Header::Serialize() const -> std::string {
    std::stringstream buffer;
    buffer << "HTTP/1.1 200 OK\r\n";
    for (const auto&[key, value] : _content) {
        buffer << std::format("{}: {}\r\n", key, value);
    }
    buffer << "\r\n";

    return buffer.str();
}