#pragma once

#include <string>
#include <string_view>
#include <functional>
#include <optional>

#include <libxml/HTMLparser.h>

xmlNode* FindFirstNodeWithTextValue(xmlNode* const root, const std::string_view content);
void ForEachNodeWithNameApplyF(xmlNode* const root, const std::string_view sv, const std::function<void(xmlNode*)>& f);
auto ConstructLexicalMonthYearFromYYYYMM(const std::string& content) -> std::string;

using href_t = std::string;
using text_t = std::string;
/** Struct representing the useful information to be extracted from an <a> node.
 * @param href the value of the href attribute
 * @param value the value of the text
*/
struct AArchiveContent_t {
    href_t href;
    text_t value;
};
std::ostream& operator<<(std::ostream& os, const AArchiveContent_t& a);
auto EvaluateANodeForArchivePage(xmlNode* const a_node) -> std::optional<AArchiveContent_t> ;
auto CollectAllMonthsInArchive(xmlNode* const root) -> std::vector<AArchiveContent_t>;

using title_t = std::string;
struct AMonthContent_t {
    href_t href;
    title_t title;
    auto operator==(const AMonthContent_t& rhs) const -> bool {
        return
            this->href == rhs.href &&
            this->title == rhs.title;
    }
};
std::ostream& operator<<(std::ostream& os, const AMonthContent_t& a);
auto EvaluateANodeForMonthPage(xmlNode* const a_node) -> std::optional<AMonthContent_t>;
auto CollectAllPhotoEntriesInMonth(xmlNode* const root, const std::string_view month_year) -> std::vector<AMonthContent_t>;

using image_href_t = href_t;
auto CollectAllImageLinksInPhotoEntry(xmlNode* const root) -> std::vector<image_href_t>;
auto ValidImageLink(const image_href_t& href) -> bool;

enum class QUALITY {
    Q_4K,
    Q_2K,
    Q_HD,
};
auto ExtractQualityImageHref(const QUALITY quality, const std::vector<image_href_t>& links) -> image_href_t;
