#include "MonthFilter.h"

MonthFilter::MonthFilter(const FilterOps op, const std::string& condition)
: _operator{op}
, _condition{condition}
, _valid_grammar{true} {
    // conditions
    // 2 characters
    if (condition.size() != 2) {
        _valid_grammar = false;
    }
    // all 2 characters are numerical types
    for (const auto c : condition) {
        if (c < '0' || c > '9') {
            _valid_grammar = false;
        }
    }
}

auto MonthFilter::Filter(const filter_target_t& target) const -> bool {
    // If invalid grammar then ignore this filter (return true)
    if (!_valid_grammar) {
        return true;
    }
    else {
        const int condition{std::stoi(_condition)};
        const int month{std::stoi(target.substr(4, 2))};
        return Compare(_operator, condition, month);
    }
}
