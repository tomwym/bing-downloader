#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "filters/IFilter.h"
#include "filters/PassConditionalFilter.h"
#include "filters/YearFilter.h"
#include "filters/MonthFilter.h"
#include "filters/DayFilter.h"

using namespace ::testing;
using std::make_unique;

TEST(FilterTests, BasicAdd)
{
    const std::vector<std::string> collection{
        "20230101",
        "20230102",
        "20230103",
        "20230601",
        "20230602",
        "20230603",
        "20231201",
        "20231202",
        "20231203",
        "20240101",
        "20240102",
        "20240103",
        "20240601",
        "20240602",
        "20240603",
        "20241201",
        "20241202",
        "20241203",
    };
    {
        PassConditionalFilter filter;
        filter.And(make_unique<YearFilter>(FilterOps::EQ, "2024"));
        for (const auto& date : collection) {
            if (filter.Apply(date))
                std::cout << date << '\n';
        }
    }
    std::cout << '\n';
    {
        PassConditionalFilter filter;
        filter.And(make_unique<YearFilter>(FilterOps::EQ, "2024"))
            .And(make_unique<MonthFilter>(FilterOps::EQ, "06"));
        for (const auto& date : collection) {
            if (filter.Apply(date))
                std::cout << date << '\n';
        }
    }
    std::cout << '\n';
    {
        PassConditionalFilter filter;
        filter.And(make_unique<YearFilter>(FilterOps::EQ, "2024"))
            .And(make_unique<MonthFilter>(FilterOps::EQ, "06"))
            .And(make_unique<DayFilter>(FilterOps::EQ, "02"));
        for (const auto& date : collection) {
            if (filter.Apply(date))
                std::cout << date << '\n';
        }
    }
}