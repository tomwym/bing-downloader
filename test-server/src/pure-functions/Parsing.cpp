#include "Parsing.h"

#include <sstream>

namespace Naive {

auto SplitString(const std::string& str, const char delimiter) -> std::vector<std::string> {
    std::stringstream ss{str};
    std::string buffer;
    std::vector<std::string> container;
    int i = 0;
    while (std::getline(ss, buffer, delimiter)) {
        container.push_back(buffer);
    }
    return container;
}

auto TrimLeadingWhitespace(std::string str) -> std::string {
    str.erase(0, str.find_first_not_of(' '));
    return str;
}

auto TrimTrailingWhitespace(std::string str) -> std::string {
    str.erase(str.find_last_not_of(' ')+1);
    return str;
}

auto TrimTrailingAndLeadingWhitespace(std::string str) -> std::string {
    return TrimTrailingWhitespace(TrimLeadingWhitespace(str));
}

auto SplitStringOnFirstDelimiter(const std::string& str, const std::string& delimiter) -> std::pair<std::string, std::string> {
    std::string::size_type n = str.find(delimiter);
    if (n == std::string::npos) {
        return {};
    }
    return { TrimTrailingAndLeadingWhitespace(str.substr(0, n)), TrimTrailingAndLeadingWhitespace(str.substr(n+1)) };
}

}
