#include <iostream>
#include <nlohmann/json.hpp>
#include <filesystem>

using json = nlohmann::json;
using std::vector;
using std::string;

int main() {
    // std::printf("Hello world");
    json j;
    j["id"] = 2.424;
    j["happy"] = true;
    j["nothing"] = nullptr;
    j["answer"]["everything"] = 42;
    json j1, j2;
    j1["good"] = "morning";
    j2["hi"] = "tom";
    vector<json> v = {j1, j2};
    j["vec"] = v;
    std::cout << j.dump(2) << std::endl;

    std::filesystem::path directory{"./src"};
    auto R = [&directory] (const json& doc) {
        // Find all files in directory

    };
    using Filename_t = std::string;
    auto CollectDirectory = [&directory]() -> std::vector<Filename_t>
    {
        std::vector<Filename_t> buffer;
        for (const auto & entry : std::filesystem::directory_iterator(directory)) {
            if (entry.is_regular_file() && entry.path().extension() == ".jpg") {
                buffer.emplace_back(entry.path().filename().string());
                std::cout << entry.path().filename().string() << '\n';
            }
        }
        return buffer;
    };
    CollectDirectory();

}