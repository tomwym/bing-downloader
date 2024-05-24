#include "FileIO.h"

#include <fstream>

auto VerifyPath(const std::filesystem::path& path) -> void {
    if (!std::filesystem::exists(path)) {
        std::filesystem::create_directory(path.parent_path());
    }
}

auto ReadFromFile(const std::filesystem::path& file) -> std::vector<char> {
    VerifyPath(file);
    std::ifstream readFile {file, std::ios::binary};
    if (readFile.fail())
        return {};
    return std::vector<char>{ std::istreambuf_iterator<char>(readFile), std::istreambuf_iterator<char>() };
}

auto WriteToFile(const std::filesystem::path& file, const std::vector<char>& contents) -> void {
    VerifyPath(file);
    std::ofstream writeFile {file, std::ios::binary | std::ios::trunc};
    if (writeFile.fail())
        return;
    writeFile.write(contents.data(), contents.size());
}

auto WriteToFile(const std::filesystem::path& file, const std::string& contents) -> void {
    VerifyPath(file);
    std::ofstream writeFile {file, std::ios::binary | std::ios::trunc};
    if (writeFile.fail())
        return;
    writeFile.write(contents.data(), contents.size());
}
