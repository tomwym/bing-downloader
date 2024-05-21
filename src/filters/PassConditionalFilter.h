#pragma once

#include "IFilter.h"

#include <vector>

class PassConditionalFilter {
    std::vector<FilterPtr> _filters;
public:
    auto And(FilterPtr filter) -> PassConditionalFilter&;
    auto Apply(const filter_target_t& target) const -> bool;
    auto Apply(const std::vector<filter_target_t>& collection) const -> std::vector<filter_target_t>;
};
