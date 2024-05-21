#include <nlohmann/json.hpp>
#include <libxml/HTMLparser.h>
#include <libxml/xpath.h>
#include <libxml/parser.h>
#include <nlohmann/json.hpp>

#include "CurlWriteChunks.h"
#include <filesystem>
#include <string>
#include <fstream>
#include <format>
#include <functional>
#include <iostream>
#include <concepts>
#include <chrono>
#include <ranges>
#include <set>

#include <stdio.h>

#include "CurlWriteChunks.h"
#include "XMLQueries.h"
#include "XMLContainer.h"

#define NULL_CHECK(x, expr) if (!x) expr;
#define MAKE_PATH(x) std::filesystem::path{x}

using json = nlohmann::json;
using std::make_unique;

class Example
{
public:
    Example(const std::string& url, const std::filesystem::path& path)
    : _url{url}, _path{path} {}
    auto URL() -> std::string { return _url; }
    auto Path() -> std::filesystem::path {return _path; };
    virtual ~Example() = default;
private:
    std::string _url;
    std::filesystem::path _path;
};

class ExampleFactory
{
public:
    static auto Smiley() -> Example {
        return {"https://i.pinimg.com/736x/d2/b1/c5/d2b1c5ef795dd4260841d5725e9a3a4b.jpg", "./panda/smiley.jpeg"};
    }
    static auto Panda() -> Example {
        return {"https://imgproxy.nanxiongnandi.com/gY_6NmiAa_t0O3GaMnXS0PqgVaztEfxKBLmlN2fcLjo/w:1920/q:100/att:1/aHR0cHM6Ly9pbWcu/bmFueGlvbmduYW5k/aS5jb20vMjAyNDAz/L0JhbWJvb1BhbmRh/LmpwZw.jpg", "./panda/panda.jpeg"};
    }

    static auto GenericExample() -> Example {
        return {"https://www.example.com/", "./panda/example.html"};
    }

    static auto BingWallpaper() -> Example {
        return {"https://bingwallpaper.anerg.com/index.html", "./out/index.html"};
    }

    static auto IndexMonth() -> Example {
        return {"https://bingwallpaper.anerg.com/archive/us/202402", "./out/202402.html"};
    }

    static auto LocalHost() -> Example {
        return {"127.0.0.1:8080", "./out/localhost.html"};
    }
};




// curl -o tetmp.jpeg https://imgproxy.nanxiongnandi.com/gY_6NmiAa_t0O3GaMnXS0PqgVaztEfxKBLmlN2fcLjo/w:1920/q:100/att:1/aHR0cHM6Ly9pbWcu/bmFueGlvbmduYW5k/aS5jb20vMjAyNDAz/L0JhbWJvb1BhbmRh/LmpwZw.jpg
int main(void)
{
    Example ex { ExampleFactory::BingWallpaper() };
    // CurlWriteChunks curl{ex.URL(), ex.Path()};

    /* Macro to check API for match with the DLL we are using */
    LIBXML_TEST_VERSION

    XMLContainerPtr home_xml{make_unique<XMLContainer>(ex.Path())};

    xmlNode* root_element = home_xml->Root();
    printf("Root Node is %s\n", root_element->name);


    auto GetGenericLocalHTMLResource = [](const std::filesystem::path& path) -> std::unique_ptr<XMLContainer> {
        XMLContainerPtr xml {};
        if (xml->Valid())
            return std::move(xml);
        return nullptr;
    };

    auto ConstructOnlineResourceURL = [](const std::string& base, const std::string& uri) -> std::string {
        return base + uri;
    };
    // 202404, 202403, 202402, 202401...
    const std::vector<AArchiveContent_t> set = CollectAllMonthsInArchive(root_element);

    auto GetOnlineResourceFromLink = [&GetGenericLocalHTMLResource](const std::string& url) -> XMLContainerPtr {
        const std::filesystem::path temp{ "./.temp/temp.html" };
        CurlWriteChunks curl {url, temp};
        if (!curl.Success()) { return nullptr; }
        XMLContainerPtr xml {make_unique<XMLContainer>(temp)};
        if (xml->Valid())
            return std::move(xml);
        return nullptr;
    };

    json doc_node;
    // iterate through each month in the archive
    for (const auto& month : set) {
        std::cout << month << '\n';
        // Get the online resource
        const std::string month_url {ConstructOnlineResourceURL(ex.URL(), month.href)};
        XMLContainerPtr month_xml{GetOnlineResourceFromLink(month_url)};
        if (!month_xml) { continue; }
        std::vector<AMonthContent_t> images_in_month {
            CollectAllPhotoEntriesInMonth(month_xml->Root(), ConstructLexicalMonthYearFromYYYYMM(month.value)) };

        json month_node;

        // now interate through each photo within the month collection
        //
        for (const auto& [i, entry] : std::ranges::reverse_view{images_in_month} | std::ranges::views::enumerate) {
            const std::string day_enumeration{std::format("{:02}", i+1)}; // 01, 02, .. 31 etc.
            const std::string year_month_and_day{month.value + day_enumeration}; // 20240401

            const std::string image_html_url {ConstructOnlineResourceURL(ex.URL(), entry.href)};
            XMLContainerPtr image_html_xml {GetOnlineResourceFromLink(image_html_url)};
            if (!image_html_xml) { continue; }
            const auto image_download_href = ExtractQualityImageHref(QUALITY::Q_HD, CollectAllImageLinksInPhotoEntry(image_html_xml->Root()));
            // std::cout << entry.href << '\n' << entry.title << '\n' << image_download_href << "\n\n";
            json day_node;

            day_node["href"] = entry.href;
            day_node["description"] = entry.title;
            day_node["url"] = image_download_href;

            month_node[year_month_and_day] = day_node;

            break;
        }

        doc_node[month.value] = month_node;
    }

    std::cout << doc_node.dump(2) << std::endl;

    /**
     * Input params
     * -d/--directory
     */


    std::filesystem::path directory{"archive"};
    using Filename_t = std::string;
    auto CollectDirectory = [&directory]() -> std::set<Filename_t>
    {
        const auto a = std::filesystem::current_path();
        std::set<Filename_t> buffer;
        for (const auto & entry : std::filesystem::directory_iterator(directory)) {
            if (entry.is_regular_file() && entry.path().extension() == ".jpg") {
                buffer.insert(entry.path().filename().string());
                std::cout << entry.path().filename().string() << '\n';
            }
        }
        return buffer;
    };
    std::set<Filename_t> existing_images{CollectDirectory()};

    auto Read = [&directory] (const json& doc) {
        // Find all files in directory
    };

    auto DownloadFiles = [](const json doc, const std::filesystem::path& target_directory, const std::set<Filename_t>& existing_items) {
        // For some href string such as /detail/us/SleepingFox, return the last token: SleepingFox
        auto HrefTokens = [](const std::string& s) -> std::string {
            std::stringstream ss{s};
            std::string segment;
            std::vector<std::string> tokens;
            while (std::getline(ss, segment, '/')) {
                tokens.emplace_back(segment);
            }
            return tokens.back();
        };
        for (const auto& month : doc) {
            for (const auto& [key, value] : month.items()) {
                // 20240101_SleepingFox.jpg
                const std::string name{std::string{key} + "_" + HrefTokens(value["href"]) + ".jpg"};
                std::cout << name << " @ " << value["url"] << '\n';
                if (!existing_items.contains(name)) {
                    std::cout << std::format("Downloaded: {}\n", name);
                    // download to directory
                    // download target_directory / name
                    CurlWriteChunks curl{value["url"], target_directory / name};
                }
                else {
                    std::cout << std::format("File already exists: {}\n", name);
                }

            }
        }
    };
    DownloadFiles(doc_node, directory, existing_images);


    xmlCleanupParser();    // Free globals
    return 0;
}

