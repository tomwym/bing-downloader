#include "PassConditionalFilter.h"

auto PassConditionalFilter::And(FilterPtr filter) -> PassConditionalFilter& {
    _filters.push_back(std::move(filter));
    return *this;
}

auto PassConditionalFilter::Apply(const filter_target_t& target) const -> bool {
    for (const auto& filter : _filters) {
        if (!filter->Filter(target))
            return false;
    }
    return true;
}

auto PassConditionalFilter::Apply(const std::vector<filter_target_t>& collection) const -> std::vector<filter_target_t> {
    std::vector<filter_target_t> buffer;
    buffer.reserve(collection.size());
    for (const auto& target : collection) {
        if (Apply(target)) {
            buffer.emplace_back(target);
        }
    }
    return buffer;
}
