#pragma once

#include <string>
#include <filesystem>
#include <memory>

class CurlWriteChunks {
public:
    CurlWriteChunks(const std::string& url, const std::filesystem::path& path);
    ~CurlWriteChunks();

    auto Success() const -> bool;
private:
    class impl;
    std::unique_ptr<impl> pImpl;
};