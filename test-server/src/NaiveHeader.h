#pragma once

#include <string>
#include <map>

enum class CONTENT_TYPE {
    IMAGE_JPEG
};

class Header {
    std::map<std::string, std::string> _content;

public:
    Header() = default;
    operator std::string() const;

    auto AddAcceptRanges() -> Header&;
    auto AddDate() -> Header&;
    auto AddContentType(const CONTENT_TYPE& content_type) -> Header&;
    auto AddContentLength(const std::size_t& size) -> Header&;
    auto AddHost(const std::string& host) -> Header&;

    auto Serialize() const -> std::string;
};