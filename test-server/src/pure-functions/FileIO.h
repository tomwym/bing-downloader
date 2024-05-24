#pragma once

#include <vector>
#include <string>
#include <filesystem>

[[nodiscard]] auto ReadFromFile(const std::filesystem::path& file) -> std::vector<char>;
auto WriteToFile(const std::filesystem::path& file, const std::vector<char>& contents) -> void;
auto WriteToFile(const std::filesystem::path& file, const std::string& contents) -> void;
