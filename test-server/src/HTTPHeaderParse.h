#pragma once

#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <sstream>

#include "pure-functions/Parsing.h"

namespace Naive {

using header_t = std::string;
using header_line_t = std::string;
using Field_t = std::string;
using Value_t = std::string;
using ServerResponse = std::map<Field_t, Value_t>;

template<typename T>
auto Print(const std::vector<T>& v) -> void {
    for (const auto& val : v) {
        std::cout << val << '\n';
    }
}

enum class METHOD {
    GET,
    HEAD,
    POST,
    PUT,
    DELETE,
    CONNECT,
    OPTIONS,
    TRACE,
};

static std::vector<Field_t> keys {
    "Host",
    "Connection",
    "Accept",
    "Referer",
};

auto ReadHead(const header_t& header) -> ServerResponse {
    std::vector<header_line_t> lines(SplitString(header, '\n'));
    return {};
}

auto GetFunction(const header_line_t& line) {

}

auto IterationAwareWhile() {

}

}