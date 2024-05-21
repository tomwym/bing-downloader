#pragma once

#include <memory>
#include <string>

using filter_target_t = std::string;

class Filter_t {
public:
    virtual auto Filter(const filter_target_t& target) const -> bool = 0;
};
using FilterPtr = std::unique_ptr<Filter_t>;