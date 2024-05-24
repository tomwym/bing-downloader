#pragma once

#include <vector>
#include <string>
#include <utility>

namespace Naive {

auto SplitString(const std::string& str, const char delimiter) -> std::vector<std::string>;
auto TrimLeadingWhitespace(std::string str) -> std::string;
auto TrimTrailingWhitespace(std::string str) -> std::string;
auto TrimTrailingAndLeadingWhitespace(std::string str) -> std::string;
auto SplitStringOnFirstDelimiter(const std::string& str, const std::string& delimiter) -> std::pair<std::string, std::string>;

}
