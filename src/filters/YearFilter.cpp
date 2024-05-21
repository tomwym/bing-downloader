#include "YearFilter.h"

YearFilter::YearFilter(const FilterOps op, const std::string& condition)
: _operator{op}
, _condition{condition}
, _valid_grammar{true} {
    // conditions
    // 4 characters
    if (condition.size() != 4) {
        _valid_grammar = false;
    }
    // all 4 characters are numerical types
    for (const auto c : condition) {
        if (c < '0' || c > '9') {
            _valid_grammar = false;
        }
    }
}

auto YearFilter::Filter(const filter_target_t& target) const -> bool {
    // If invalid grammar then ignore this filter (return true)
    if (!_valid_grammar) {
        return true;
    }
    else {
        const int condition{std::stoi(_condition)};
        const int year{std::stoi(target.substr(0, 4))};
        return Compare(_operator, condition, year);
    }
}
