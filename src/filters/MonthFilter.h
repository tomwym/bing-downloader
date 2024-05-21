#pragma once 

#include "IFilter.h"
#include "FilterOps.h"

class MonthFilter : public Filter_t {
    FilterOps _operator;
    std::string _condition;
    bool _valid_grammar;
public:
    MonthFilter(const FilterOps op, const std::string& condition);
    auto Filter(const filter_target_t& target) const -> bool final;
};
