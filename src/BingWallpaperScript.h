#pragma once

#include <vector>
#include <ranges>

#include "ResourceGetter.h"
#include "XMLContainer.h"
#include "XMLQueries.h"
#include "CurlGetDispatcher.h"
#include "filters/PassConditionalFilter.h"
#include "filters/YearFilter.h"
#include "filters/MonthFilter.h"
#include "filters/DayFilter.h"
#include <nlohmann/json.hpp>

#include <fstream>

using json = nlohmann::json;

class BingWallpaperScript {
    std::filesystem::path _image_output_dir;
    std::filesystem::path _supplemental_resources_dir;
    std::unique_ptr<ResourceGetter> _resource_getter;
    const std::string _url;
    const std::string _index;
    std::unique_ptr<PassConditionalFilter> _filter;
    std::unique_ptr<CurlGetDispatcher> _dispatcher;
public:
    BingWallpaperScript(const std::filesystem::path& output, const std::filesystem::path& supplement, const unsigned int thread_c, const bool dry_run, std::unique_ptr<PassConditionalFilter>&& filter)
    : _image_output_dir{output}
    , _supplemental_resources_dir{supplement}
    , _resource_getter{std::make_unique<ResourceGetter>(dry_run, _supplemental_resources_dir)}
    , _url{"https://bingwallpaper.anerg.com"}
    , _index{"https://bingwallpaper.anerg.com/index.html"}
    , _filter{std::move(filter)}
    , _dispatcher{std::make_unique<CurlGetDispatcher>(dry_run, thread_c)}
    {
    }

    auto Execute() -> void {
        XMLContainerPtr index_xml{_resource_getter->GetResource(_index, RESOURCE_TYPE::HTML)};
        if (!index_xml || !index_xml->Valid()) {
            std::cout << std::format("No index found.\n");
            return;
        }
        json doc_node;
        const std::vector<AArchiveContent_t> all_months_in_archive{CollectAllMonthsInArchive(index_xml->Root())};

        ForEachMonth(doc_node, all_months_in_archive);
        std::ofstream manifest_file( _image_output_dir / "_manifest.json");
        manifest_file << doc_node.dump(2);

    }

private:
    auto ConstructOnlineResourceURL(const std::string& base, const std::string& uri) -> std::string {
        return base + uri;
    }

    auto ForEachMonth(json& doc_node, const std::vector<AArchiveContent_t>& all_months) -> void {
        for (const auto& month : all_months) {
            if (_filter->Apply(month.value)) {
                std::cout << std::format("DoForEachMonth for... {} {}\n", month.href, month.value);
                DoForEachMonth(doc_node, month);
            }
        }
    }
    auto DoForEachMonth(json& doc_node, const AArchiveContent_t& month_content) -> void {

        const std::string month_url {ConstructOnlineResourceURL(_url, month_content.href)};
        XMLContainerPtr month_xml {_resource_getter->GetResource(month_url, RESOURCE_TYPE::HTML)};
        if (!month_xml)
            return;

        std::vector<AMonthContent_t> images_in_month {
            CollectAllPhotoEntriesInMonth(month_xml->Root(), ConstructLexicalMonthYearFromYYYYMM(month_content.value)) };

        json month_node;
        ForEachDay(month_node, images_in_month, month_content.value);
        doc_node[month_content.value] = month_node;
    }

    auto ForEachDay(json& month_node, const std::vector<AMonthContent_t>& all_days, const std::string& month_numeric_value /*202404*/) -> void {
        for (const auto& [i, day] : std::ranges::reverse_view{all_days} | std::ranges::views::enumerate) {
            const std::string day_enumeration{std::format("{:02}", i+1)}; // 01, 02, .. 31 etc.
            const std::string year_month_and_day_numeric_value{month_numeric_value + day_enumeration}; // 20240401

            if (_filter->Apply(year_month_and_day_numeric_value)) {
                DoForEachDay(month_node, day, year_month_and_day_numeric_value);
            }
        }
    }

    auto DoForEachDay(json& month_node, const AMonthContent_t& day, const std::string& day_numeric_value /*20240401*/) -> void {
        const std::string image_html_url {ConstructOnlineResourceURL(_url, day.href)};
        XMLContainerPtr image_html_xml {_resource_getter->GetResource(image_html_url, RESOURCE_TYPE::HTML)};
        if (!image_html_xml) {
            return;
        }

        const auto image_jpg_href = ExtractQualityImageHref(
            QUALITY::Q_4K, CollectAllImageLinksInPhotoEntry(image_html_xml->Root()));

        // href == /detail/us/SleepingFox.jpg -> SleepingFox.jpg
        auto HrefFinalToken = [](const std::string& s) -> std::string {
            std::stringstream ss{s};
            std::string segment;
            std::vector<std::string> tokens;
            while (std::getline(ss, segment, '/')) {
                tokens.emplace_back(segment);
            }
            return tokens.back();
        };
        const std::filesystem::path filename{ _image_output_dir / std::format("{}_{}.jpg", day_numeric_value, HrefFinalToken(day.href))};
        if (std::filesystem::exists(filename)) {
            std::cout << std::format("{} already exists!\n", filename.string());
        } else {
            // if (filename.string().size() == 0) {
            //     int a = 0;
            //     int b = a;
            // }
            // auto redo = false;
            // CurlWriteChunks curl{image_jpg_href, filename};
            // if (!curl.Success()) {
            //     std::cout << std::format("Unsuccessful on {}\n", filename.string());
            // }
            // std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            _dispatcher->Add(std::make_unique<DownloadInfo>(image_jpg_href, filename));
        }

        json day_node;
        day_node["href"] = day.href;
        day_node["description"] = day.title;
        day_node["url"] = image_jpg_href;
        month_node[day_numeric_value] = day_node;
    }

};