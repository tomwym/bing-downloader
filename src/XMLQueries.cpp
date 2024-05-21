#include "XMLQueries.h"

#include <libxml/HTMLparser.h>

#include <iostream>
#include <functional>
#include <chrono>
#include <cassert>
#include <map>

using namespace std::chrono;

#define STR(str) reinterpret_cast<const char*>(str)

std::ostream& operator<<(std::ostream& os, const AArchiveContent_t& a) {
    os << a.href << " " << a.value << '\n';
    return os;
}

std::ostream& operator<<(std::ostream& os, const AMonthContent_t& a) {
    os << a.href << " " << a.title << '\n';
    return os;
}

/** Finds the first text node which contains some given string.
 * @param root the initial node to begin the search
 * @param content the search string within some <node>string</node>
 */
xmlNode* FindFirstNodeWithTextValue(xmlNode* const root, const std::string_view content) {
    if (root == nullptr)
        return nullptr;

    for (xmlNode* current = root; current; current = current->next) {
        if (current->type == XML_TEXT_NODE && current->content && (const char *)current->content == content) {
            return current->parent;
        }
        xmlNode* result = FindFirstNodeWithTextValue(current->children, content);
        if (result != nullptr)
        {
            return result;
        }
    }
    return nullptr;
}

void ForEachNodeWithNameApplyF(xmlNode* const root, const std::string_view sv, const std::function<void(xmlNode*)>& f) {
    if (root == nullptr)
        return;

    for (xmlNode* current = root; current; current = current->next) {
        if (current->name && (const char*)current->name == sv)
            f(current);

        ForEachNodeWithNameApplyF(current->children, sv, f);
    }
}

/** Parse the input string to return a well formatted date.
 * @param input string formatted as YYYYMM
 * @return well formed month and year i.e. January 2024
 */
auto ConstructLexicalMonthYearFromYYYYMM(const std::string& content) -> std::string {
    std::string year = content.substr(0, 4);
    std::string month = content.substr(4);
    const year_month ym = {std::chrono::year(std::stoi(year)), std::chrono::month(std::stoi(month))};
    return std::format("{:%B} {:%Y}", ym.month(), ym.year());
}

/** Given some <a> node, return a struct of type containing the value of the href attribute and the content of the text child node.
 * @param a_node xml node representing the <a> node within the DOM
 * @returns a AArchiveContent_t struct containing the value of the href and the text i.e. <a href="href_value">text_value<\a>
 */
auto EvaluateANodeForArchivePage(xmlNode* const a_node) -> std::optional<AArchiveContent_t> {
    if (!a_node)
        return {};

    for(auto prop = a_node->properties; prop; prop = prop->next) {
        std::string name = STR(prop->name);
        if (name == "href") {
            // std::cout << prop->name << " value: " << prop->children->content << '\n';
            AArchiveContent_t temp {STR(prop->children->content), STR(a_node->children->content)};
            //std::cout << temp;
            ConstructLexicalMonthYearFromYYYYMM(temp.value);
            return std::make_optional(temp);
        }
    }
    assert(false);
    return std::nullopt;
}

/** Given a root node to some document, this function will find all archived months.
 * @param root the root node to a document
 * @return a vector of month data
 */
auto CollectAllMonthsInArchive(xmlNode* const root) -> std::vector<AArchiveContent_t> {
    xmlNode* const archive_node = FindFirstNodeWithTextValue(root, "Archive");
    std::vector<AArchiveContent_t> archive_set;
    auto CollectAllArchiveANodes = [&archive_set](xmlNode* const a_node) {
        std::optional<AArchiveContent_t> result{EvaluateANodeForArchivePage(a_node)};
        if (result)
            archive_set.emplace_back(result.value());
    };
    ForEachNodeWithNameApplyF(archive_node, "a", CollectAllArchiveANodes);
    return archive_set;
}

/**
 * @param a_node xml node representing the <a> node within the DOM
 * @returns a AMonthContent_t struct containing the value of the href and the data-bs-title attributes i.e. <a href="href_value" data-bs-title="title_value\>
 */
auto EvaluateANodeForMonthPage(xmlNode* const a_node) -> std::optional<AMonthContent_t> {
    if (!a_node)
        return {};
    AMonthContent_t temp{};

    for(auto prop = a_node->properties; prop; prop = prop->next) {
        const std::string name {STR(prop->name)};
        if (name == "href") {
            temp.href = STR(prop->children->content);
        }
        if (name == "data-bs-title") {
            temp.title = STR(prop->children->content);
        }
    }
    if (temp == AMonthContent_t{}) {
        assert(false);
        return std::nullopt;
    }
    return temp;
}


/**
 * @param
 * @return
 */
auto CollectAllPhotoEntriesInMonth(xmlNode* const root, const std::string_view month_year) -> std::vector<AMonthContent_t> {
    xmlNode* const month_node = FindFirstNodeWithTextValue(root, month_year);
    std::vector<AMonthContent_t> image_set;
    auto CollectAllArchiveANodes = [&image_set](xmlNode* const a_node) {
        std::optional<AMonthContent_t> result{EvaluateANodeForMonthPage(a_node)};
        if (result)
            image_set.emplace_back(result.value());
    };
    ForEachNodeWithNameApplyF(month_node, "a", CollectAllArchiveANodes);
    return image_set;
}

/**
 * This function extracts the three image links from any image html.
 * @param root The root node of the html containing the image info.
 */
auto CollectAllImageLinksInPhotoEntry(xmlNode* const root) -> std::vector<image_href_t> {
    if (!root) return {};
    std::vector<image_href_t> hrefs;
    auto collect_hrefs = [&hrefs](xmlNode* node) -> void {
        // std::cout << "node: " << node->name << ' ';
        for(auto prop = node->properties; prop; prop = prop->next) {
            std::string name = (const char*)(prop->name);
            if (name == "href") {
                // std::cout << prop->name << ' ' << (const char*)prop->children->content << ' ' << (const char*)node->children->content << '\n';
                const image_href_t href {(const char*)prop->children->content};
                if(ValidImageLink(href))
                    hrefs.push_back(href);
            }
        }
    };
    ForEachNodeWithNameApplyF(root, "a", collect_hrefs);
    return hrefs;
}

/**
 * Returns true if the link is a suspected image link.
 * General heuristic follows that the front begins with "https://" and tail ends in one of {".jpg", ".jpeg", ".png"}
 */
auto ValidImageLink(const image_href_t& href) -> bool {
    bool valid{false};
    static const std::string prefix{"https://"};
    const std::size_t size{href.size()};
    static const std::vector<std::string> suffixes{".jpg", ".jpeg", ".png"};

    if (href.find(prefix) == std::string::npos)
        return false;
    for (const auto suffix : suffixes) {
        if (href.find(suffix) != std::string::npos) {
            valid = true;
        }
    }
    return valid;
}

auto ExtractQualityImageHref(const QUALITY quality, const std::vector<image_href_t>& links) -> image_href_t {
    static const std::map<QUALITY, std::string> QUALITIES {
        {QUALITY::Q_4K, "w:3840"},
        {QUALITY::Q_2K, "w:2560"},
        {QUALITY::Q_HD, "w:1920"},
    };

    // First try the requested quality. If it is unavailable then try the other options.
    const std::string requested_quality{QUALITIES.at(quality)};
    for (const auto& href : links) {
        if (href.contains(requested_quality)) {
            return href;
        }
    }

    // If the requested quality is not found, then try again with all the qualities.
    for (const auto&[_, value] : QUALITIES) {
        for (const auto& href : links) {
            if (href.contains(value)) {
                return href;
            }
        }
    }
    return {};
}