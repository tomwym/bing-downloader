#include "CurlWriteChunks.h"
#include <string>
#include <filesystem>
#include <iostream>
#include <format>
#include <fstream>
#include <curl/curl.h>



auto VerifyPath(const std::filesystem::path& path) {
    if (!std::filesystem::exists(path)) {
        auto parent = path.parent_path();
        std::filesystem::create_directories(parent);
        // std::cout << std::format("Created path @ {}.\n", path.parent_path().string());
    }
    // std::cout << std::format("Path: {} exists.\n", path.parent_path().string());
}

class CurlWriteChunks::impl {
    CURL* _curl;
    bool _successful;
public:
    impl(const std::string& url, const std::filesystem::path& path)
    : _curl{nullptr}
    , _successful{false}
    {
        std::cout << std::format("Requesting from url: {}\n", url);
        VerifyPath(path);
        std::ofstream file{path, std::ios::binary | std::ios::trunc};

        auto WriteChunk = [](const char* ptr, const size_t size, const size_t nmemb, void* stream) -> size_t {
            std::ofstream* fstream {reinterpret_cast<std::ofstream*>(stream)};
            if (!fstream)
                std::cout << "Bad cast >:(\n";
            fstream->write(ptr, size*nmemb);
            // std::cout << "Wrote: " << size*nmemb << '\n';
            return size*nmemb;
        };

        CURLcode res;
        _curl = curl_easy_init();
        if (_curl) {
            curl_easy_setopt(_curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, +WriteChunk);
            curl_easy_setopt(_curl, CURLOPT_WRITEDATA, reinterpret_cast<void*>(&file));

            /* example.com is redirected, so we tell libcurl to follow redirection */
            //curl_easy_setopt(_curl, CURLOPT_FOLLOWLOCATION, 1L);

            /* Perform the request, res gets the return code */
            res = curl_easy_perform(_curl);
            /* Check for errors */
            if(res == CURLE_OK) {
                _successful = true;
                char *ct;
                /* ask for the content-type */
                res = curl_easy_getinfo(_curl, CURLINFO_CONTENT_TYPE, &ct);
            
                if((CURLE_OK == res) && ct) {
                    // printf("We received Content-Type: %s\n", ct);
                }
            }
            else
                std::cerr << std::format("curl_easy_perform() failed: {}\n", curl_easy_strerror(res));
        }
    }

    ~impl() {
        curl_easy_cleanup(_curl);
        curl_global_cleanup();
    }

    auto Success() const -> bool {
        return _successful;
    }
};

CurlWriteChunks::CurlWriteChunks(const std::string& url, const std::filesystem::path& path)
: pImpl{std::make_unique<CurlWriteChunks::impl>(url, path)} 
{
}

CurlWriteChunks::~CurlWriteChunks() = default;

auto CurlWriteChunks::Success() const -> bool {
    return pImpl->Success();
}
